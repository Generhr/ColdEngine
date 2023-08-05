#pragma once

#include "EngineException.h"
#include "Colors.h"
#include "GDIPlusManager.h"
#include "LinearAlgebra/Vector2.h"
#include "LinearAlgebra/Vector3.h"
#include "Surface.h"
#include "TexVertex.h"

#define byte                                                                                                           \
    win_byte_override  // https://stackoverflow.com/questions/45957830/gdipluspath-throws-ambiguous-byte-for-cstddef-and-rpcndr-h

#include <d3d11.h>

#undef byte

#include <wrl.h>

#define COLD_GFX_EXCEPTION(hr, note) Graphics::Exception(hr, note, _CRT_WIDE(__FILE__), __LINE__)

class Graphics {
public:
    class Exception : public EngineException {
    public:
        Exception(HRESULT hr, const std::wstring& note, const wchar_t* file, unsigned int line);
        std::wstring GetErrorName() const;
        std::wstring GetErrorDescription() const;
        std::wstring GetFullMessage() const override;
        std::wstring GetExceptionType() const override;

    private:
        HRESULT hr;
    };

private:
    // Vertex format for the framebuffer fullscreen textured quad
    struct FSQVertex {
        float x, y, z;  // Position
        float u, v;     // Texcoords
    };

public:
    explicit Graphics(class HWNDKey& key);

    ~Graphics();

    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;
    Graphics& operator=(Graphics&&) = delete;

    void EndFrame();

    void BeginFrame();

    void PutPixel(int x, int y, Color c) {
        sysBuffer.PutPixel(x, y, c);
    }

    void PutPixel(int x, int y, int r, int g, int b) {
        PutPixel(x, y, {static_cast<unsigned char>(r), static_cast<unsigned char>(g), static_cast<unsigned char>(b)});
    }

    void DrawLine(float x1, float y1, float x2, float y2, Color c) {
        const float dx = x2 - x1;
        const float dy = y2 - y1;

        if (dy == 0.0f && dx == 0.0f) {
            PutPixel(static_cast<int>(x1), static_cast<int>(y1), c);
        }
        else if (abs(dy) > abs(dx)) {
            if (dy < 0.0f) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            const float m = dx / dy;
            float y = y1;
            int lastIntY = 0;

            for (float x = x1; y < y2; y += 1.0f, x += m) {
                lastIntY = static_cast<int>(y);

                PutPixel(static_cast<int>(x), lastIntY, c);
            }

            if (static_cast<int>(y2) > lastIntY) {
                PutPixel(static_cast<int>(x2), static_cast<int>(y2), c);
            }
        }
        else {
            if (dx < 0.0f) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            const float m = dy / dx;
            float x = x1;
            int lastIntX = 0;

            for (float y = y1; x < x2; x += 1.0f, y += m) {
                lastIntX = static_cast<int>(x);

                PutPixel(lastIntX, static_cast<int>(y), c);
            }

            if (static_cast<int>(x2) > lastIntX) {
                PutPixel(static_cast<int>(x2), static_cast<int>(y2), c);
            }
        }
    }

    void DrawLine(const Vec3& p1, const Vec3& p2, Color c) {
        DrawLine(p1[0], p1[1], p2[0], p2[1], c);
    }

    void DrawTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, Color c);
    void DrawTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);
    void DrawTriangleTexWrap(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);

private:
    void DrawFlatTopTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, Color c);
    void DrawFlatBottomTriangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, Color c);
    void DrawFlatTopTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);
    void DrawFlatBottomTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);
    void DrawFlatTriangleTex(const TexVertex& v0,
        const TexVertex& v1,
        const TexVertex& v2,
        const Surface& tex,
        const TexVertex& dv0,
        const TexVertex& dv1,
        TexVertex& itEdge1);
    void DrawFlatTopTriangleTexWrap(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);
    void DrawFlatBottomTriangleTexWrap(const TexVertex& v0,
        const TexVertex& v1,
        const TexVertex& v2,
        const Surface& tex);
    void DrawFlatTriangleTexWrap(const TexVertex& v0,
        const TexVertex& v1,
        const TexVertex& v2,
        const Surface& tex,
        const TexVertex& dv0,
        const TexVertex& dv1,
        TexVertex& itEdge1);

private:
    GDIPlusManager gdipMan;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pImmediateContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pSysBufferTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSysBufferTextureView;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;
    D3D11_MAPPED_SUBRESOURCE mappedSysBufferTexture;
    Surface sysBuffer;

public:
    static constexpr unsigned int ScreenWidth = 640u;
    static constexpr unsigned int ScreenHeight = 640u;
};
