#pragma once
//------------------------------------------------------------------------------
/**
    @class Oryol::_priv::d3d12Texture
    @ingroup _priv
    @brief D3D12 implementation of class texture
*/
#include "Resource/Id.h"
#include "Gfx/Resource/textureBase.h"
#include "Gfx/d3d12/d3d12_decl.h"

namespace Oryol {
namespace _priv {

class blub {
    Id blob;
};

class d3d12Texture : public textureBase {
public:
    /// constructor
    d3d12Texture();
    /// destructor
    ~d3d12Texture();

    /// clear the object
    void Clear();

    /// the d3d12 texture resource object 
    ID3D12Resource* d3d12TextureRes;
    /// optional depth-buffer resource object
    ID3D12Resource* d3d12DepthBufferRes;
    /// need to keep track of texture resource state
    D3D12_RESOURCE_STATES d3d12TextureState;
    /// need to keep track of depth-buffer resource state
    D3D12_RESOURCE_STATES d3d12DepthBufferState;
    /// optional render-target-view descriptor-heap slot-index
    int rtvDescriptorSlot;
    /// optional depth-stencil-view descriptor-heap slot-index
    int dsvDescriptorSlot;
};

} // namespace _priv
} // namespace Oryol
