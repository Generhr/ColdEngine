#pragma once

#include "Pipeline.h"
#include "Shaders/DefaultVertexShader.h"
#include "Shaders/DefaultGeometryShader.h"

// Flat shading with face normals calculated in gs
class GeometryFlatEffect {
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

    // Calculate color based on face normal calculated from cross product of geometry - no interpolation of color
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
            // Calculate face normal
            const auto n = Vec3::CrossProduct((in1.pos - in0.pos), (in2.pos - in0.pos)).GetNormalized();
            // Calculate intensity based on angle of incidence
            const auto d = diffuse * std::max(0.0f, -n * dir);
            // Add diffuse + ambient, filter by material color, saturate and scale
            const auto c = Color(color.GetHadamard(d + ambient).Saturate() * 255.0f);

            return {{in0.pos, c}, {in1.pos, c}, {in2.pos, c}};
        }

        void SetDiffuseLight(const Vec3& c) {
            diffuse = {c[0], c[1], c[2]};
        }

        void SetAmbientLight(const Vec3& c) {
            ambient = {c[0], c[1], c[2]};
        }

        void SetLightDirection(const Vec3& dl) {
            assert(dl.GetMagnitudeSquared() >= 0.001f);

            dir = dl.GetNormalized();
        }

        void SetMaterialColor(Color c) {
            color = Vec3(c);
        }

    private:
        Mat3 rotation;
        Vec3 translation;
        Vec3 dir = {0.0f, 0.0f, 1.0f};
        // This is the intensity if direct light from source color light so need values per color component
        Vec3 diffuse = {1.0f, 1.0f, 1.0f};
        // This is intensity of indirect light that bounces off other obj in scene color light so need values per color
        // component
        Vec3 ambient = {0.1f, 0.1f, 0.1f};
        // Color of material (how much light of each color is reflected)
        Vec3 color = {0.8f, 0.85f, 1.0f};
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
