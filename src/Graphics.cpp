#include "Graphics.h"

#include "DXErr.h"
#include "MainWindow.h"

#include "LinearAlgebra/Vector2.h"

#include "Utility/ReadData.h"

#include <array>
#include <cassert>
#include <functional>
#include <string>

#pragma comment(lib, "d3d11.lib")


using Microsoft::WRL::ComPtr;

Graphics::Graphics(HWNDKey& key) : sysBuffer(ScreenWidth, ScreenHeight) {
    assert(key.hWnd != nullptr);

    // Create device and swap chain/get render target view
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = Graphics::ScreenWidth;
    sd.BufferDesc.Height = Graphics::ScreenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 1;
    sd.BufferDesc.RefreshRate.Denominator = 60;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = key.hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    const D3D_FEATURE_LEVEL featureLevelsRequested = D3D_FEATURE_LEVEL_9_1;
    const UINT numLevelsRequested = 1;
    D3D_FEATURE_LEVEL featureLevelsSupported;
    HRESULT hr = NULL;
    const UINT createFlags = 0u;
#ifdef _DEBUG
#ifdef USE_DIRECT3D_DEBUG_RUNTIME
    createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif

    auto pixelShaderBytes = GraphicsUtils::ReadData(L"ShaderOutput\\FramebufferPS.cso");
    auto vertexShaderBytes = GraphicsUtils::ReadData(L"ShaderOutput\\FramebufferVS.cso");

    // Create device and front/back buffers
    if (FAILED(hr = D3D11CreateDeviceAndSwapChain(nullptr,
                   D3D_DRIVER_TYPE_HARDWARE,
                   nullptr,
                   createFlags,
                   &featureLevelsRequested,
                   numLevelsRequested,
                   D3D11_SDK_VERSION,
                   &sd,
                   &pSwapChain,
                   &pDevice,
                   &featureLevelsSupported,
                   &pImmediateContext))) {
        throw COLD_GFX_EXCEPTION(hr, L"Creating device and swap chain");
    }

    // Get handle to backbuffer
    ComPtr<ID3D11Resource> pBackBuffer;

    if (FAILED(hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), static_cast<LPVOID*>(&pBackBuffer)))) {
        throw COLD_GFX_EXCEPTION(hr, L"Getting back buffer");
    }

    // Create a view on backbuffer that we can render to
    if (FAILED(hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView))) {
        throw COLD_GFX_EXCEPTION(hr, L"Creating render target view on backbuffer");
    }

    // Set backbuffer as the render target using created view
    pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);

    // Set viewport dimensions
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<float>(Graphics::ScreenWidth);
    vp.Height = static_cast<float>(Graphics::ScreenHeight);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    pImmediateContext->RSSetViewports(1, &vp);

    // Create texture for cpu render target
    D3D11_TEXTURE2D_DESC sysTexDesc;
    sysTexDesc.Width = Graphics::ScreenWidth;
    sysTexDesc.Height = Graphics::ScreenHeight;
    sysTexDesc.MipLevels = 1;
    sysTexDesc.ArraySize = 1;
    sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sysTexDesc.SampleDesc.Count = 1;
    sysTexDesc.SampleDesc.Quality = 0;
    sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
    sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    sysTexDesc.MiscFlags = 0;

    // Create the texture
    if (FAILED(hr = pDevice->CreateTexture2D(&sysTexDesc, nullptr, &pSysBufferTexture))) {
        throw COLD_GFX_EXCEPTION(hr, L"Creating sysbuffer texture");
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = sysTexDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    // Create the resource view on the texture
    if (FAILED(hr = pDevice->CreateShaderResourceView(pSysBufferTexture.Get(), &srvDesc, &pSysBufferTextureView))) {
        throw COLD_GFX_EXCEPTION(hr, L"Creating view on sysBuffer texture");
    }

    // Create pixel shader for framebuffer
    if (FAILED(
            hr =
                pDevice->CreatePixelShader(pixelShaderBytes.data(), pixelShaderBytes.size(), nullptr, &pPixelShader))) {
        throw COLD_GFX_EXCEPTION(hr, L"Creating pixel shader");
    }

    // Create vertex shader for framebuffer
    if (FAILED(hr = pDevice->CreateVertexShader(vertexShaderBytes.data(),
                   vertexShaderBytes.size(),
                   nullptr,
                   &pVertexShader))) {
        throw COLD_GFX_EXCEPTION(hr, L"Creating vertex shader");
    }

    // Create and fill vertex buffer with quad for rendering frame
    const FSQVertex vertices[] = {
        {-1.0f, 1.0f, 0.5f, 0.0f, 0.0f},
        {1.0f, 1.0f, 0.5f, 1.0f, 0.0f},
        {1.0f, -1.0f, 0.5f, 1.0f, 1.0f},
        {-1.0f, 1.0f, 0.5f, 0.0f, 0.0f},
        {1.0f, -1.0f, 0.5f, 1.0f, 1.0f},
        {-1.0f, -1.0f, 0.5f, 0.0f, 1.0f},
    };
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(FSQVertex) * 6;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0u;
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    if (FAILED(hr = pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer))) {
        throw COLD_GFX_EXCEPTION(hr, L"Creating vertex buffer");
    }

    // Create input layout for fullscreen quad
    const D3D11_INPUT_ELEMENT_DESC ied[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}};

    // Ignore the intellisense error "namespace has no member"
    if (FAILED(
            hr = pDevice
                     ->CreateInputLayout(ied, 2, vertexShaderBytes.data(), vertexShaderBytes.size(), &pInputLayout))) {
        throw COLD_GFX_EXCEPTION(hr, L"Creating input layout");
    }

    // Create sampler state for fullscreen textured quad
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    if (FAILED(hr = pDevice->CreateSamplerState(&sampDesc, &pSamplerState))) {
        throw COLD_GFX_EXCEPTION(hr, L"Creating sampler state");
    }
}

Graphics::~Graphics() {
    // Clear the state of the device context before destruction
    if (pImmediateContext) {
        pImmediateContext->ClearState();
    }
}

void Graphics::BeginFrame() {
    sysBuffer.Clear(Colors::Red);
}

void Graphics::EndFrame() {
    HRESULT hr;

    // Lock and map the adapter memory for copying over the sysbuffer
    if (FAILED(hr = pImmediateContext
                        ->Map(pSysBufferTexture.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture))) {
        throw COLD_GFX_EXCEPTION(hr, L"Mapping sysbuffer");
    }

    // Perform the copy line-by-line
    sysBuffer.Present(mappedSysBufferTexture.RowPitch, reinterpret_cast<BYTE*>(mappedSysBufferTexture.pData));
    // Release the adapter memory
    pImmediateContext->Unmap(pSysBufferTexture.Get(), 0u);

    // Render offscreen scene texture to back buffer
    pImmediateContext->IASetInputLayout(pInputLayout.Get());
    pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
    pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
    pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    const UINT stride = sizeof(FSQVertex);
    const UINT offset = 0u;
    pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
    pImmediateContext->PSSetShaderResources(0u, 1u, pSysBufferTextureView.GetAddressOf());
    pImmediateContext->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());
    pImmediateContext->Draw(6u, 0u);

    // Flip back/front buffers
    if (FAILED(hr = pSwapChain->Present(1u, 0u))) {
        throw COLD_GFX_EXCEPTION(hr, L"Presenting back buffer");
    }
}

Graphics::Exception::Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line)
    : EngineException(file, line, note), hr(hr) {
}

std::wstring Graphics::Exception::GetFullMessage() const {
    const std::wstring empty = L"";
    const std::wstring errorName = GetErrorName();
    const std::wstring errorDesc = GetErrorDescription();
    const std::wstring& note = GetNote();
    const std::wstring location = GetLocation();

    return (!errorName.empty() ? std::wstring(L"Error: ") + errorName + L"\n" : empty) +
           (!errorDesc.empty() ? std::wstring(L"Description: ") + errorDesc + L"\n" : empty) +
           (!note.empty() ? std::wstring(L"Note: ") + note + L"\n" : empty) +
           (!location.empty() ? std::wstring(L"Location: ") + location : empty);
}

std::wstring Graphics::Exception::GetErrorName() const {
    return DXGetErrorString(hr);
}

std::wstring Graphics::Exception::GetErrorDescription() const {
    std::array<wchar_t, 512> wideDescription;
    DXGetErrorDescription(hr, wideDescription.data(), wideDescription.size());

    return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const {
    return L"Cold Graphics Exception";
}
