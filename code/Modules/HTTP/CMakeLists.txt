#-------------------------------------------------------------------------------
#   oryol HTTP module
#-------------------------------------------------------------------------------
oryol_begin_module(HTTP)
oryol_sources(. base)
oryol_sources_osx(osx)
oryol_sources_linux(curl)
oryol_sources_windows(windows)
oryol_deps(Core Messaging IO)
oryol_end_module()

oryol_begin_unittest(HTTP)
oryol_sources(UnitTests)
oryol_deps(Core Messaging IO HTTP)
oryol_frameworks_osx(Foundation)
oryol_end_unittest()