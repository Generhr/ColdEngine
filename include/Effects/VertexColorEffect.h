#pragma once

#include "Pipeline.h"
#include "Shaders/DefaultVertexShader.h"


// Color gradient effect between vertices
class VertexColorEffect {
public:
    // The vertex type that will be input into the pipeline
    class Vertex {
    public:
        Vertex() = default;

        explicit Vertex(const Vec3& pos) : pos(pos) {
        }

        Vertex(const Vec3& pos, const Vertex& src) : pos(pos), color(src.color) {
        }

        Vertex(const Vec3& pos, const Vec3& color) : pos(pos), color(color) {
        }

        Vertex& operator+=(const Vertex& rhs) {
            pos += rhs.pos;
            color += rhs.color;
            return *this;
        }

        Vertex operator+(const Vertex& rhs) const {
            return Vertex(*this) += rhs;
        }

        Vertex& operator-=(const Vertex& rhs) {
            pos -= rhs.pos;
            color -= rhs.color;
            return *this;
        }

        Vertex operator-(const Vertex& rhs) const {
            return Vertex(*this) -= rhs;
        }

        Vertex& operator*=(float rhs) {
            pos *= rhs;
            color *= rhs;
            return *this;
        }

        Vertex operator*(float rhs) const {
            return Vertex(*this) *= rhs;
        }

        Vertex& operator/=(float rhs) {
            pos /= rhs;
            color /= rhs;
            return *this;
        }

        Vertex operator/(float rhs) const {
            return Vertex(*this) /= rhs;
        }

    public:
        Vec3 pos;
        Vec3 color;
    };

    // Default vs rotates and translates vertices, does not touch attributes
    typedef DefaultVertexShader<Vertex> VertexShader;

    // Invoked for each pixel of a triangle takes an input of attributes that are the result of interpolating vertex
    // attributes and outputs a color
    class PixelShader {
    public:
        template<class Input>
        Color operator()(const Input& in) const {
            return Color(in.color);
        }
    };

public:
    VertexShader vs;
    PixelShader ps;
};
