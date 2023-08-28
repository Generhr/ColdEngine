#pragma once

#include "Pipeline.h"
#include "Shaders/DefaultVertexShader.h"
#include "Shaders/DefaultGeometryShader.h"

// Flat shading with vertex normals
class VertexFlatEffect {
public:
    // The vertex type that will be input into the pipeline
    class Vertex {
    public:
        Vertex() = default;

        explicit Vertex(const Vec3& pos) : pos(pos) {
        }

        Vertex(const Vec3& pos, const Vertex& src) : pos(pos), n(src.n) {
        }

        Vertex(const Vec3& pos, const Vec3& n) : pos(pos), n(n) {
        }

        Vertex& operator+=(const Vertex& rhs) {
            pos += rhs.pos;
            n += rhs.n;

            return *this;
        }

        Vertex operator+(const Vertex& rhs) const {
            return Vertex(*this) += rhs;
        }

        Vertex& operator-=(const Vertex& rhs) {
            pos -= rhs.pos;
            n -= rhs.n;

            return *this;
        }

        Vertex operator-(const Vertex& rhs) const {
            return Vertex(*this) -= rhs;
        }

        Vertex& operator*=(float rhs) {
            pos *= rhs;
            n *= rhs;

            return *this;
        }

        Vertex operator*(float rhs) const {
            return Vertex(*this) *= rhs;
        }

        Vertex& operator/=(float rhs) {
            pos /= rhs;
            n /= rhs;

            return *this;
        }

        Vertex operator/(float rhs) const {
            return Vertex(*this) /= rhs;
        }

    public:
        Vec3 pos;
        Vec3 n;
    };

    // Calculate color based on normal to light angle, no interpolation of color attribute
    class VertexShader {
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
        void BindRotation(const Mat3& rotation_in) {
            rotation = rotation_in;
        }

        void BindTranslation(const Vec3& translation_in) {
            translation = translation_in;
        }

        Output operator()(const Vertex& v) const {
            // Calculate intensity based on angle of incidence
            const auto d = diffuse * std::max(0.0f, -(v.n * rotation) * dir);
            // Add diffuse + ambient, filter by material color, saturate and scale
            const auto c = color.GetHadamard(d + ambient).Saturate() * 255.0f;

            return {v.pos * rotation + translation, Color(c)};
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
        Vec3 diffuse = {1.0f, 1.0f, 1.0f};
        Vec3 ambient = {0.1f, 0.1f, 0.1f};
        Vec3 color = {0.8f, 0.85f, 1.0f};
    };

    // Default gs passes vertices through and outputs triangle
    typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

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
