#pragma once

#include "Pipeline.h"
#include "Shaders/DefaultVertexShader.h"
#include "Shaders/DefaultGeometryShader.h"


// Basic texture effect
class TextureEffect {
public:
    // The vertex type that will be input into the pipeline
    class Vertex {
    public:
        Vertex() = default;

        explicit Vertex(const Vec3& pos) : pos(pos) {
        }

        Vertex(const Vec3& pos, const Vertex& src) : pos(pos), t(src.t) {
        }

        Vertex(const Vec3& pos, const Vec2& t) : pos(pos), t(t) {
        }

        Vertex& operator+=(const Vertex& rhs) {
            pos += rhs.pos;
            t += rhs.t;
            return *this;
        }

        Vertex operator+(const Vertex& rhs) const {
            return Vertex(*this) += rhs;
        }

        Vertex& operator-=(const Vertex& rhs) {
            pos -= rhs.pos;
            t -= rhs.t;
            return *this;
        }

        Vertex operator-(const Vertex& rhs) const {
            return Vertex(*this) -= rhs;
        }

        Vertex& operator*=(float rhs) {
            pos *= rhs;
            t *= rhs;
            return *this;
        }

        Vertex operator*(float rhs) const {
            return Vertex(*this) *= rhs;
        }

        Vertex& operator/=(float rhs) {
            pos /= rhs;
            t /= rhs;
            return *this;
        }

        Vertex operator/(float rhs) const {
            return Vertex(*this) /= rhs;
        }

    public:
        Vec3 pos;
        Vec2 t;
    };

    // Default vs rotates and translates vertices, does not touch attributes
    typedef DefaultVertexShader<Vertex> VertexShader;
    // Default gs passes vertices through and outputs triangle
    typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

    // Invoked for each pixel of a triangle takes an input of attributes that are the result of interpolating vertex
    // attributes and outputs a color
    class PixelShader {
    public:
        PixelShader() : pTex(), tex_width(), tex_height(), tex_xclamp(), tex_yclamp() {
        }

        template<class Input>
        Color operator()(const Input& in) const {
            return pTex->GetPixel((unsigned int)std::min(in.t[0] * tex_width + 0.5f, tex_xclamp),
                (unsigned int)std::min(in.t[1] * tex_height + 0.5f, tex_yclamp));
        }

        void BindTexture(const std::wstring& filename) {
            pTex = std::make_unique<Surface>(Surface::FromFile(filename));
            tex_width = float(pTex->GetWidth());
            tex_height = float(pTex->GetHeight());
            tex_xclamp = tex_width - 1.0f;
            tex_yclamp = tex_height - 1.0f;
        }

    private:
        std::unique_ptr<Surface> pTex;
        float tex_width;
        float tex_height;
        float tex_xclamp;
        float tex_yclamp;
    };

public:
    VertexShader vs;
    GeometryShader gs;
    PixelShader ps;
};
