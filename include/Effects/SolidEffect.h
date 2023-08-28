#pragma once

#include "Pipeline.h"
#include "Shaders/DefaultVertexShader.h"
#include "Shaders/DefaultGeometryShader.h"

// Solid color attribute not interpolated
class SolidEffect {
public:
    // The vertex type that will be input into the pipeline
    class Vertex {
    public:
        Vertex() = default;

        explicit Vertex(const Vec3& pos) : pos(pos) {
        }

        Vertex(const Vec3& pos, const Vertex& src) : pos(pos), color(src.color) {
        }

        Vertex(const Vec3& pos, const Color& color) : pos(pos), color(color) {
        }

        Vertex& operator+=(const Vertex& rhs) {
            pos += rhs.pos;
            return *this;
        }

        Vertex operator+(const Vertex& rhs) const {
            return Vertex(*this) += rhs;
        }

        Vertex& operator-=(const Vertex& rhs) {
            pos -= rhs.pos;

            return *this;
        }
        Vertex operator-(const Vertex& rhs) const {
            return Vertex(*this) -= rhs;
        }

        Vertex& operator*=(float rhs) {
            pos *= rhs;
            return *this;
        }

        Vertex operator*(float rhs) const {
            return Vertex(*this) *= rhs;
        }

        Vertex& operator/=(float rhs) {
            pos /= rhs;
            return *this;
        }

        Vertex operator/(float rhs) const {
            return Vertex(*this) /= rhs;
        }

    public:
        Vec3 pos;
        Color color;
    };

    // Default vs rotates and translates vertices, does not touch attributes
    typedef DefaultVertexShader<Vertex> VertexShader;
    // Default gs passes vertices through and outputs triangle
    typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

    // Invoked for each pixel of a triangle takes an input of attributes that are the result of interpolating vertex
    // attributes and outputs a color
    class PixelShader {
    public:
        template<class I>
        Color operator()(const I& in) const {
            return in.color;
        }
    };

public:
    VertexShader vs;
    GeometryShader gs;
    PixelShader ps;
};
