'''
Code generator for message protocol xml files.
'''

import os
import sys
import yaml
import genutil as util

Version = 14 
    
#-------------------------------------------------------------------------------
def writeHeaderTop(f, desc) :
    '''
        Write header area for the generated C++ header.
    '''
    f.write('#pragma once\n')
    f.write('//-----------------------------------------------------------------------------\n')
    f.write('/* #version:{}#\n'.format(Version))
    f.write('    machine generated, do not edit!\n')
    f.write('*/\n')
    f.write('#include <cstring>\n')

#-------------------------------------------------------------------------------
def writeIncludes(f, desc) :
    '''
        Write include statements in the generated C++ header.
    '''
    f.write('#include "Messaging/Message.h"\n')
    parentHdr = desc.get('parentProtocolHeader', 'Messaging/Protocol.h')
    f.write('#include "{}"\n'.format(parentHdr))

    for hdr in desc.get('headers', []) :
        f.write('#include "{}"\n'.format(hdr))
    f.write('\n')

#-------------------------------------------------------------------------------
def writeProtocolMethods(f, desc) :
    '''
    Write the protocol methods
    '''
    f.write('    static ProtocolIdType GetProtocolId() {\n')
    f.write("        return '{}';\n".format(desc['id']))
    f.write('    };\n')

#-------------------------------------------------------------------------------
def writeMessageIdEnum(f, desc) :
    '''
    Write the enum with message ids
    '''
    protocol = desc['name']
    parentProtocol = desc.get('parentProtocol', 'Protocol')

    f.write('    class MessageId {\n')
    f.write('    public:\n')
    f.write('        enum {\n')
    msgCount = 0
    for msg in desc['messages'] :
        if msgCount == 0:
            f.write('            ' + msg['name'] + 'Id = ' + parentProtocol + '::MessageId::NumMessageIds, \n')
        else :
            f.write('            ' + msg['name'] + 'Id,\n')
        msgCount += 1
    f.write('            NumMessageIds\n')
    f.write('        };\n')
    f.write('        static const char* ToString(MessageIdType c) {\n')
    f.write('            switch (c) {\n')
    for msg in desc['messages'] :
        msgName = msg['name'] + 'Id'
        f.write('                case ' + msgName + ': return "' + msgName + '";\n')
    f.write('                default: return "InvalidMessageId";\n')
    f.write('            }\n')
    f.write('        };\n')
    f.write('        static MessageIdType FromString(const char* str) {\n')
    for msg in desc['messages'] :
        msgName = msg['name'] + 'Id'
        f.write('            if (std::strcmp("' + msgName + '", str) == 0) return ' + msgName + ';\n')
    f.write('            return InvalidMessageId;\n')
    f.write('        };\n')
    f.write('    };\n')
    f.write('    typedef Ptr<Message> (*CreateCallback)();\n')
    f.write('    static CreateCallback jumpTable[' + protocol + '::MessageId::NumMessageIds];\n')

#-------------------------------------------------------------------------------
def writeFactoryClassDecl(f, desc) :
    '''
    Writes the message factory for this protocol
    '''
    f.write('    class Factory {\n')
    f.write('    public:\n')
    f.write('        static Ptr<Message> Create(MessageIdType id);\n')
    f.write('    };\n')

#-------------------------------------------------------------------------------
def writeFactoryClassImpl(f, desc) :
    '''
    Writes the factory class implementation
    '''
    protocol = desc['name']
    parentProtocol = desc.get('parentProtocol', 'Protocol')

    f.write(protocol + '::CreateCallback ' + protocol + '::jumpTable[' + protocol + '::MessageId::NumMessageIds] = { \n') 
    for msg in desc['messages'] :
        f.write('    &' + protocol + '::' + msg['name'] + '::FactoryCreate,\n')
    f.write('};\n')
    f.write('Ptr<Message>\n')
    f.write(protocol + '::Factory::Create(MessageIdType id) {\n')
    f.write('    if (id < ' + parentProtocol + '::MessageId::NumMessageIds) {\n')
    f.write('        return ' + parentProtocol + '::Factory::Create(id);\n')
    f.write('    }\n')
    f.write('    else {\n')
    f.write('        o_assert(id < ' + protocol + '::MessageId::NumMessageIds);\n')
    f.write('        return jumpTable[id - ' + parentProtocol + '::MessageId::NumMessageIds]();\n')
    f.write('    };\n')
    f.write('}\n')

#-------------------------------------------------------------------------------
def getAttrDefaultValue(attr) :
    '''
    Get the default value for a given attribute
    '''
    defValue = attr.get('default')
    attrType = attr['type']
    if attrType in ('int8', 'int16', 'int32', 'int64', 'uint8', 'uint16', 'uint32', 'uint64') :
        if not defValue :
            defValue = '0'
    elif attrType in ('char', 'unsigned char', 'int', 'unsigned int', 'short', 'unsigned short', 'long', 'unsigned long') :
        if not defValue :
            defValue = '0'
    elif attrType == 'bool' :
        if not defValue :
            defValue = 'false'
    elif attrType in ('float32', 'float') :
        if not defValue :
            defValue = '0.0f'
    elif attrType in ('float64', 'double') :
        if not defValue :
            defValue = '0.0'
    return defValue;

#-------------------------------------------------------------------------------
def getRefType(attrType) :
    '''
    Get the reference type string for an attribute type
    ''' 
    if attrType in ('int8', 'int16', 'int32', 'int64', 'uint8', 'uint16', 'uint32', 'uint64') :
        return attrType
    elif attrType in ('bool', 'char', 'unsigned char', 'int', 'unsigned int', 'short', 'unsigned short', 'long', 'unsigned long') :
        return attrType
    elif attrType in ('float32', 'float') :
        return attrType
    elif attrType in ('float64', 'double') :
        return attrType
    else :
        return 'const ' + attrType + '&'

#-------------------------------------------------------------------------------
def getValueType(attrType) :
    '''
    Get the value type string for an attribute type
    ''' 
    return attrType

#-------------------------------------------------------------------------------
def isArrayType(attrType) :
    '''
    Test if the type string is an array type (Array<TYPE>)
    '''
    return attrType.startswith('Array<') and attrType.endswith('>')

#-------------------------------------------------------------------------------
def getArrayType(attrType) :
    '''
    Get the element type of an array type.
    '''
    # strip the 'Array<' at the left, and the '>' at the right
    return attrType[6:-1]

#-------------------------------------------------------------------------------
def writeMessageClasses(f, desc) :
    '''
    Write the message classes to the generated C++ header
    '''
    protocolId = desc['id']
    for msg in desc['messages'] :
        msgClassName = msg['name']
        msgParentClassName = msg.get('parent', 'Message')
        f.write('    class ' + msgClassName + ' : public ' + msgParentClassName + ' {\n')
        f.write('        OryolClassDecl(' + msgClassName + ');\n')
        f.write('        OryolTypeDecl(' + msgClassName + ',' + msgParentClassName + ');\n')
        f.write('    public:\n')

        # write constructor
        f.write('        ' + msgClassName + '() {\n')
        f.write('            this->msgId = MessageId::' + msgClassName + 'Id;\n')
        for attr in msg.get('attrs', []) :
            attrName = attr['name']
            defValue = getAttrDefaultValue(attr)
            if defValue :
                f.write('            this->' + attrName + ' = ' + defValue + ';\n')
        f.write('        };\n')

        # special factory create method
        f.write('        static Ptr<Message> FactoryCreate() {\n')
        f.write('            return Create();\n')
        f.write('        };\n')

        # special class message id static method
        f.write('        static MessageIdType ClassMessageId() {\n')
        f.write('            return MessageId::' + msgClassName + 'Id;\n')
        f.write('        };\n')

        # virtual method which checks whether the method belongs to a protocol
        f.write('        virtual bool IsMemberOf(ProtocolIdType protId) const override {\n')
        f.write("            if (protId == '" + protocolId + "') return true;\n")
        f.write('            else return ' + msgParentClassName + '::IsMemberOf(protId);\n')
        f.write('        };\n') 

        # write members
        for attr in msg.get('attrs', []) :
            attrName = attr['name']
            attrType = attr['type']
            f.write('        ' + getValueType(attrType) + ' ' + attrName + ';\n')
        f.write('    };\n')

#-------------------------------------------------------------------------------
def generateHeader(desc, absHeaderPath) :
    '''
    Generate the C++ header file 
    '''
    f = open(absHeaderPath, 'w')

    protocol = desc['name']
    writeHeaderTop(f, desc) 
    writeIncludes(f, desc)
    f.write('namespace Oryol {\n')
    f.write('class ' + protocol + ' {\n')
    f.write('public:\n')
    writeProtocolMethods(f, desc)
    writeMessageIdEnum(f, desc)
    writeFactoryClassDecl(f, desc)
    writeMessageClasses(f, desc)
    f.write('};\n')
    f.write('}\n')
    f.close()

#-------------------------------------------------------------------------------
def writeSourceTop(f, desc, absSourcePath) :
    '''
    Write source file header area
    '''
    path, hdrFileAndExt = os.path.split(absSourcePath)
    hdrFile, ext = os.path.splitext(hdrFileAndExt)

    f.write('//-----------------------------------------------------------------------------\n')
    f.write('// #version:{}# machine generated, do not edit!\n'.format(Version))
    f.write('//-----------------------------------------------------------------------------\n')
    f.write('#include "Pre.h"\n')
    f.write('#include "' + hdrFile + '.h"\n')
    f.write('\n')

#-------------------------------------------------------------------------------
def generateSource(desc, absSourcePath) :
    '''
    Generate the C++ source file
    '''
    protocol = desc['name']

    f = open(absSourcePath, 'w')
    writeSourceTop(f, desc, absSourcePath)
    f.write('namespace Oryol {\n')
    for msg in desc['messages'] :
        msgClassName = msg['name']
        f.write('OryolClassImpl(' + protocol + '::' + msgClassName + ');\n')
        
    writeFactoryClassImpl(f, desc)
    f.write('}\n')
    f.close()

#-------------------------------------------------------------------------------
def generate(input, out_src, out_hdr) :
    if util.isDirty(Version, [input], [out_src, out_hdr]) :
        with open(input, 'r') as f :
            desc = yaml.load(f)
        generateHeader(desc, out_hdr)
        generateSource(desc, out_src)

