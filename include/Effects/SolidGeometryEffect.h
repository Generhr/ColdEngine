#pragma once

#include "Pipeline.h"
#include "Shaders/DefaultVertexShader.h"

// Solid color attribute taken from table in gs and not interpolated
class SolidGeometryEffect {
public:
    // The vertex type that will be input into the pipeline
    class Vertex {
    public:
        Vertex() = default;

        explicit Vertex(const Vec3& pos) : pos(pos) {
        }

        Vertex(const Vec3& pos, const Vertex& src) : pos(pos) {
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
    };

    // Default vs rotates and translates vertices, does not touch attributes
    typedef DefaultVertexShader<Vertex> VertexShader;

    // GeometryShader colors vertices using their index from a table, every two triangles are colored from the same
    // entry
    class GeometryShader {
    public:
        class Output {
        public:
            Output() = default;

            explicit Output(const Vec3& pos) : pos(pos) {
            }

            Output(const Vec3& pos, const Output& src) : pos(pos), color(src.color) {
            }

            Output(const Vec3& pos, const Color& color) : pos(pos), color(color) {
            }

            Output& operator+=(const Output& rhs) {
                pos += rhs.pos;

                return *this;
            }

            Output operator+(const Output& rhs) const {
                return Output(*this) += rhs;
            }

            Output& operator-=(const Output& rhs) {
                pos -= rhs.pos;

                return *this;
            }

            Output operator-(const Output& rhs) const {
                return Output(*this) -= rhs;
            }

            Output& operator*=(float rhs) {
                pos *= rhs;

                return *this;
            }

            Output operator*(float rhs) const {
                return Output(*this) *= rhs;
            }

            Output& operator/=(float rhs) {
                pos /= rhs;

                return *this;
            }

            Output operator/(float rhs) const {
                return Output(*this) /= rhs;
            }

        public:
            Vec3 pos;
            Color color;
        };

    public:
        Triangle<Output> operator()(const VertexShader::Output& in0,
            const VertexShader::Output& in1,
            const VertexShader::Output& in2,
            size_t triangle_index) const {
            return {{in0.pos, triangle_colors[triangle_index / 2]},
                {in1.pos, triangle_colors[triangle_index / 2]},
                {in2.pos, triangle_colors[triangle_index / 2]}};
        };

        void BindColors(std::vector<Color> colors) {
            triangle_colors = std::move(colors);
        }

    private:
        std::vector<Color> triangle_colors;
    };

    // Invoked for each pixel of a triangle takes an input of attributes that are the result of interpolating vertex
    // attributes and outputs a color
    class PixelShader {
    public:
        template<class Input>
        Color operator()(const Input& in) const {
            return in.color;
        }
    };

public:
    VertexShader vs;
    GeometryShader gs;
    PixelShader ps;
};
