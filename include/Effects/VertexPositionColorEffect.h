#pragma once

#include "Pipeline.h"
#include "Shaders/DefaultGeometryShader.h"

// Color gradient effect between vertices determined by vertex position
class VertexPositionColorEffect {
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

    // Uses `x, y, z` position components to determine color
    class VertexShader {
    public:
        class Output {
        public:
            Output() = default;

            explicit Output(const Vec3& pos) : pos(pos) {
            }

            Output(const Vec3& pos, const Output& src) : pos(pos), color(src.color) {
            }

            Output(const Vec3& pos, const Vec3& color) : pos(pos), color(color) {
            }

            Output& operator+=(const Output& rhs) {
                pos += rhs.pos;
                color += rhs.color;

                return *this;
            }

            Output operator+(const Output& rhs) const {
                return Output(*this) += rhs;
            }

            Output& operator-=(const Output& rhs) {
                pos -= rhs.pos;
                color -= rhs.color;

                return *this;
            }

            Output operator-(const Output& rhs) const {
                return Output(*this) -= rhs;
            }

            Output& operator*=(float rhs) {
                pos *= rhs;
                color *= rhs;

                return *this;
            }

            Output operator*(float rhs) const {
                return Output(*this) *= rhs;
            }

            Output& operator/=(float rhs) {
                pos /= rhs;
                color /= rhs;

                return *this;
            }

            Output operator/(float rhs) const {
                return Output(*this) /= rhs;
            }

        public:
            Vec3 pos;
            Vec3 color;
        };

    public:
        void BindRotation(const Mat3& rotation_in) {
            rotation = rotation_in;
        }

        void BindTranslation(const Vec3& translation_in) {
            translation = translation_in;
        }

        Output operator()(const Vertex& in) const {
            const auto pos = in.pos * rotation + translation;
            return {pos, Vec3 {std::abs(pos[0]), std::abs(pos[1]), std::abs(pos[2])} * 255.0f};
        }

    private:
        Mat3 rotation;
        Vec3 translation;
    };

    // Default gs passes vertices through and outputs triangle
    typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

    // Converts float color into packed byte color
    class PixelShader {
    public:
        template<class Input>
        Color operator()(const Input& in) const {
            return Color(in.color);
        }
    };

public:
    VertexShader vs;
    GeometryShader gs;
    PixelShader ps;
};
