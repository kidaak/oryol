//------------------------------------------------------------------------------
//  d3d11DrawStateFactory.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "d3d11DrawStateFactory.h"
#include "d3d11_impl.h"
#include "d3d11Types.h"
#include "Gfx/Core/renderer.h"
#include "Gfx/Resource/resourcePools.h"

namespace Oryol {
namespace _priv {

//------------------------------------------------------------------------------
d3d11DrawStateFactory::d3d11DrawStateFactory() :
d3d11Device(nullptr) {
    // empty
}

//------------------------------------------------------------------------------
d3d11DrawStateFactory::~d3d11DrawStateFactory() {
    o_assert_dbg(nullptr == this->d3d11Device);
}

//------------------------------------------------------------------------------
void
d3d11DrawStateFactory::Setup(const gfxPointers& ptrs) {
    o_assert_dbg(nullptr == this->d3d11Device);

    drawStateFactoryBase::Setup(ptrs);
    this->d3d11Device = this->pointers.renderer->d3d11Device;
    o_assert_dbg(nullptr != this->d3d11Device);
}

//------------------------------------------------------------------------------
void
d3d11DrawStateFactory::Discard() {
    o_assert_dbg(nullptr != this->d3d11Device);
    this->d3d11Device = nullptr;
    drawStateFactoryBase::Discard();
}

//------------------------------------------------------------------------------
ResourceState::Code
d3d11DrawStateFactory::SetupResource(drawState& ds) {
    o_assert_dbg(nullptr != this->d3d11Device);
    o_assert_dbg(nullptr == ds.d3d11InputLayout);
    o_assert_dbg(nullptr == ds.d3d11VertexShader);
    o_assert_dbg(nullptr == ds.d3d11PixelShader);
    o_assert_dbg(nullptr == ds.d3d11RasterizerState);
    o_assert_dbg(nullptr == ds.d3d11BlendState);
    o_assert_dbg(nullptr == ds.d3d11DepthStencilState);
    HRESULT hr;

    drawStateFactoryBase::SetupResource(ds);
    o_assert_dbg(ds.shd);

    // set vertex buffers, strides, offsets and input layout
    D3D11_INPUT_ELEMENT_DESC d3d11Comps[VertexAttr::NumVertexAttrs] = { 0 };
    int d3d11IASlotIndex = 0;
    for (int mshIndex = 0; mshIndex < GfxConfig::MaxNumInputMeshes; mshIndex++) {
        const mesh* msh = ds.meshes[mshIndex];
        if (msh) {
            o_assert_dbg(msh->d3d11VertexBuffer);
            ds.d3d11IAVertexBuffers[d3d11IASlotIndex] = msh->d3d11VertexBuffer;
            ds.d3d11IAStrides[d3d11IASlotIndex] = msh->vertexBufferAttrs.Layout.ByteSize();
            ds.d3d11IAOffsets[d3d11IASlotIndex] = 0;
            d3d11IASlotIndex++;
        }
    }
    // input-layout, vertex and pixel shader
    ds.d3d11InputLayout = this->createInputLayout(ds);
    ds.d3d11VertexShader = ds.shd->getVertexShaderByMask(ds.Setup.ShaderSelectionMask);
    ds.d3d11PixelShader = ds.shd->getPixelShaderByMask(ds.Setup.ShaderSelectionMask);
    o_assert_dbg(ds.d3d11InputLayout);
    o_assert_dbg(ds.d3d11VertexShader);
    o_assert_dbg(ds.d3d11PixelShader);

    // create state objects (NOTE: creating the same state will return
    // the same d3d11 state object, so no need to implement our own reuse)
    const RasterizerState& rastState = ds.Setup.RasterizerState;
    D3D11_RASTERIZER_DESC rastDesc;
    Memory::Clear(&rastDesc, sizeof(rastDesc));
    rastDesc.FillMode = D3D11_FILL_SOLID;
    rastDesc.CullMode = d3d11Types::asCullMode(rastState.CullFaceEnabled, rastState.CullFace);
    rastDesc.FrontCounterClockwise = FALSE;  // OpenGL convention
    rastDesc.DepthBias = 0;
    rastDesc.DepthBiasClamp = 0.0f;
    rastDesc.DepthClipEnable = TRUE;
    rastDesc.ScissorEnable = rastState.ScissorTestEnabled;
    rastDesc.MultisampleEnable = rastState.SampleCount > 1;
    rastDesc.AntialiasedLineEnable = FALSE;
    hr = this->d3d11Device->CreateRasterizerState(&rastDesc, &ds.d3d11RasterizerState);
    o_assert(SUCCEEDED(hr));
    o_assert_dbg(nullptr != ds.d3d11RasterizerState);

    const DepthStencilState& dsState = ds.Setup.DepthStencilState;
    D3D11_DEPTH_STENCIL_DESC dsDesc;
    Memory::Clear(&dsDesc, sizeof(dsDesc));
    dsDesc.DepthEnable = TRUE;      // FIXME: have DepthTestEnable in RasterizerState?
    dsDesc.DepthWriteMask = dsState.DepthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    dsDesc.DepthFunc = d3d11Types::asComparisonFunc(dsState.DepthCmpFunc);
    dsDesc.StencilEnable    = dsState.StencilEnabled;
    dsDesc.StencilReadMask  = dsState.StencilReadMask;
    dsDesc.StencilWriteMask = dsState.StencilWriteMask;
    dsDesc.FrontFace.StencilFailOp = d3d11Types::asStencilOp(dsState.StencilFront.FailOp);
    dsDesc.FrontFace.StencilDepthFailOp = d3d11Types::asStencilOp(dsState.StencilFront.DepthFailOp);
    dsDesc.FrontFace.StencilPassOp = d3d11Types::asStencilOp(dsState.StencilFront.PassOp);
    dsDesc.FrontFace.StencilFunc = d3d11Types::asComparisonFunc(dsState.StencilFront.CmpFunc);
    dsDesc.BackFace.StencilFailOp = d3d11Types::asStencilOp(dsState.StencilBack.FailOp);
    dsDesc.BackFace.StencilDepthFailOp = d3d11Types::asStencilOp(dsState.StencilBack.DepthFailOp);
    dsDesc.BackFace.StencilPassOp = d3d11Types::asStencilOp(dsState.StencilBack.PassOp);
    dsDesc.BackFace.StencilFunc = d3d11Types::asComparisonFunc(dsState.StencilBack.CmpFunc);
    hr = this->d3d11Device->CreateDepthStencilState(&dsDesc, &ds.d3d11DepthStencilState);
    o_assert(SUCCEEDED(hr));
    o_assert_dbg(nullptr != ds.d3d11DepthStencilState);

    const BlendState& blendState = ds.Setup.BlendState;
    D3D11_BLEND_DESC blendDesc;
    Memory::Clear(&blendDesc, sizeof(blendDesc));
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;
    blendDesc.RenderTarget[0].BlendEnable = blendState.BlendEnabled;
    blendDesc.RenderTarget[0].SrcBlend = d3d11Types::asBlendFactor(blendState.SrcFactorRGB);
    blendDesc.RenderTarget[0].DestBlend = d3d11Types::asBlendFactor(blendState.DstFactorRGB);
    blendDesc.RenderTarget[0].BlendOp = d3d11Types::asBlendOp(blendState.OpRGB);
    blendDesc.RenderTarget[0].SrcBlendAlpha = d3d11Types::asBlendFactor(blendState.SrcFactorAlpha);
    blendDesc.RenderTarget[0].DestBlendAlpha = d3d11Types::asBlendFactor(blendState.DstFactorAlpha);
    blendDesc.RenderTarget[0].BlendOpAlpha = d3d11Types::asBlendOp(blendState.OpAlpha);
    blendDesc.RenderTarget[0].RenderTargetWriteMask = d3d11Types::asColorWriteMask(blendState.ColorWriteMask);
    hr = this->d3d11Device->CreateBlendState(&blendDesc, &ds.d3d11BlendState);
    o_assert(SUCCEEDED(hr));
    o_assert_dbg(nullptr != ds.d3d11BlendState);

    return ResourceState::Valid;
}

//------------------------------------------------------------------------------
void
d3d11DrawStateFactory::DestroyResource(drawState& ds) {
    this->pointers.renderer->invalidateDrawState();
    if (nullptr != ds.d3d11InputLayout) {
        this->releaseInputLayout(ds.d3d11InputLayout);
    }
    if (nullptr != ds.d3d11RasterizerState) {
        ds.d3d11RasterizerState->Release();
    }
    if (nullptr != ds.d3d11BlendState) {
        ds.d3d11BlendState->Release();
    }
    if (nullptr != ds.d3d11DepthStencilState) {
        ds.d3d11DepthStencilState->Release();
    }
    drawStateFactoryBase::DestroyResource(ds);
}

//------------------------------------------------------------------------------
ID3D11InputLayout*
d3d11DrawStateFactory::createInputLayout(const drawState& ds) {
    o_assert_dbg(nullptr == ds.d3d11InputLayout);

    // this is trying to reuse existing input layouts which match the
    // provided mesh-vertex and vertex-shader-input layout
    // the resulting number of input layouts should be fairly small
    // (number of unique mesh vertex layout times number of unique shader vertex layouts)

    // build combined vertex layout of all input vertex buffers
    VertexLayout vertexLayout;
    for (int mshIndex = 0; mshIndex < GfxConfig::MaxNumInputMeshes; mshIndex++) {
        const mesh* msh = ds.meshes[mshIndex];
        if (msh) {
            vertexLayout.Append(msh->vertexBufferAttrs.Layout);
        }
    }
    const VertexLayout& vsInputLayout = ds.shd->Setup.VertexShaderInputLayout(ds.Setup.ShaderSelectionMask);
    const uint64 hash = VertexLayout::CombinedHash(vertexLayout, vsInputLayout);
    if (this->d3d11InputLayouts.Contains(hash)) {
        // re-use an existing input layout object
        ID3D11InputLayout* d3d11InputLayout = this->d3d11InputLayouts[hash];
        d3d11InputLayout->AddRef();
        return d3d11InputLayout;
    }
    else {
        // create a new input layout object and add to cache
        D3D11_INPUT_ELEMENT_DESC d3d11Comps[VertexAttr::NumVertexAttrs] = { 0 };
        int d3d11CompIndex = 0;
        int d3d11IASlotIndex = 0;
        for (int mshIndex = 0; mshIndex < GfxConfig::MaxNumInputMeshes; mshIndex++) {
            const mesh* msh = ds.meshes[mshIndex];
            if (msh) {
                const VertexLayout& layout = msh->vertexBufferAttrs.Layout;
                for (int compIndex = 0; compIndex < layout.NumComponents(); compIndex++, d3d11CompIndex++) {
                    const auto& comp = layout.ComponentAt(compIndex);
                    o_assert_dbg(d3d11CompIndex < VertexAttr::NumVertexAttrs);
                    D3D11_INPUT_ELEMENT_DESC& d3d11Comp = d3d11Comps[d3d11CompIndex];
                    d3d11Comp.SemanticName = d3d11Types::asSemanticName(comp.Attr);
                    d3d11Comp.SemanticIndex = d3d11Types::asSemanticIndex(comp.Attr);
                    d3d11Comp.Format = d3d11Types::asInputElementFormat(comp.Format);
                    d3d11Comp.InputSlot = d3d11IASlotIndex;
                    d3d11Comp.AlignedByteOffset = layout.ComponentByteOffset(compIndex);
                    d3d11Comp.InputSlotClass = d3d11Types::asInputClassification(msh->vertexBufferAttrs.StepFunction);
                    if (VertexStepFunction::PerVertex == msh->vertexBufferAttrs.StepFunction) {
                        d3d11Comp.InstanceDataStepRate = 0;
                    }
                    else {
                        d3d11Comp.InstanceDataStepRate = msh->vertexBufferAttrs.StepRate;
                    }
                }
                d3d11IASlotIndex++;
            }
        }

        // lookup the vertex shader bytecode
        const int32 progIndex = ds.shd->getProgIndexByMask(ds.Setup.ShaderSelectionMask);
        const void* vsByteCode = nullptr;
        uint32 vsSize = 0;
        ds.shd->Setup.VertexShaderByteCode(progIndex, ShaderLang::HLSL5, vsByteCode, vsSize);
        o_assert_dbg(vsByteCode && (vsSize > 0));

        // create d3d11 input layout object
        ID3D11InputLayout* d3d11InputLayout = nullptr;
        HRESULT hr = this->d3d11Device->CreateInputLayout(
            d3d11Comps,         // pInputElementDesc 
            d3d11CompIndex,     // NumElements
            vsByteCode,         // pShaderBytecodeWithInputSignature 
            vsSize,             // BytecodeLength
            &d3d11InputLayout);
        o_assert(SUCCEEDED(hr));
        o_assert_dbg(nullptr != d3d11InputLayout);

        // add new input layout object to cache
        o_assert(!this->d3d11InputLayouts.Contains(hash));
        this->d3d11InputLayouts.Add(hash, d3d11InputLayout);

        return d3d11InputLayout;
    }
}

//------------------------------------------------------------------------------
void
d3d11DrawStateFactory::releaseInputLayout(ID3D11InputLayout* d3d11InputLayout) {
    o_assert_dbg(d3d11InputLayout);

    // release object and check if we need to remove the cache entry
    if (0 == d3d11InputLayout->Release()) {
        // find input layout, we assume that the overall number of input
        // layouts will remain small (that's the whole point of the cache)
        //
        // NOTE: the actual input layout object will already be gone at this
        // point, don't call methods on it!
        for (int i = this->d3d11InputLayouts.Size() - 1; i >= 0; i--) {
            if (this->d3d11InputLayouts.ValueAtIndex(i) == d3d11InputLayout) {
                this->d3d11InputLayouts.EraseIndex(i);
                return;
            }
        }
        // fallthrough: input layout not found, this is a critical error, should never happen
        o_error("d3d11DrawStateFactory::releaseInputLayout(): layout doesn't exist in cache!\n");
    }
}

} // namespace _priv
} // namespace Oryol
