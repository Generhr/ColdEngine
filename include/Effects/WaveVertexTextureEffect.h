#pragma once

#include "Pipeline.h"
#include "Shaders/DefaultGeometryShader.h"

class WaveVertexTextureEffect {
public:
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

    // Perturbs vertices in y axis in sin wave based on x position and time
    class VertexShader {
    public:
        typedef Vertex Output;

    public:
        void BindRotation(const Mat3& rotation_in) {
            rotation = rotation_in;
        }

        void BindTranslation(const Vec3& translation_in) {
            translation = translation_in;
        }

        Output operator()(const Vertex& in) const {
            Vec3 pos = in.pos * rotation + translation;
            pos[1] += amplitude * std::sin(time * freqScroll + pos[0] * freqWave);

            return {pos, in.t};
        }

        void SetTime(float t) {
            time = t;
        }

    private:
        Mat3 rotation;
        Vec3 translation;
        float time = 0.0f;
        float freqWave = 10.0f;
        float freqScroll = 5.0f;
        float amplitude = 0.05f;
    };

    // Calculate lighting intensity based on light direction and a face normal computed from geometry with cross product
    class GeometryShader {
    public:
        class Output {
        public:
            Output() = default;

            explicit Output(const Vec3& pos) : pos(pos) {
            }

            Output(const Vec3& pos, const Output& src) : pos(pos), t(src.t), l(src.l) {
            }

            Output(const Vec3& pos, const Vec2& t, float l) : pos(pos), t(t), l(l) {
            }

            Output& operator+=(const Output& rhs) {
                pos += rhs.pos;
                t += rhs.t;

                return *this;
            }

            Output operator+(const Output& rhs) const {
                return Output(*this) += rhs;
            }

            Output& operator-=(const Output& rhs) {
                pos -= rhs.pos;
                t -= rhs.t;

                return *this;
            }

            Output operator-(const Output& rhs) const {
                return Output(*this) -= rhs;
            }

            Output& operator*=(float rhs) {
                pos *= rhs;
                t *= rhs;

                return *this;
            }

            Output operator*(float rhs) const {
                return Output(*this) *= rhs;
            }

            Output& operator/=(float rhs) {
                pos /= rhs;
                t /= rhs;

                return *this;
            }

            Output operator/(float rhs) const {
                return Output(*this) /= rhs;
            }

        public:
            Vec3 pos;
            Vec2 t;
            float l = 0.0;
        };

    public:
        Triangle<Output> operator()(const VertexShader::Output& in0,
            const VertexShader::Output& in1,
            const VertexShader::Output& in2,
            size_t triangle_index) const {
            // Calculate face normal
            const auto n = Vec3::CrossProduct((in1.pos - in0.pos), (in2.pos - in0.pos)).GetNormalized();
            // Calculate intensity based on angle of incidence plus ambient and saturate
            const auto l = std::min(1.0f, diffuse * std::max(0.0f, -n * dir) + ambient);

            return {{in0.pos, in0.t, l}, {in1.pos, in1.t, l}, {in2.pos, in2.t, l}};
        }

        void SetDiffuseLight(float d) {
            diffuse = d;
        }

        void SetAmbientLight(float a) {
            ambient = a;
        }

        void SetLightDirection(const Vec3& dl) {
            assert(dl.GetMagnitudeSquared() >= 0.001f);
            dir = dl.GetNormalized();
        }

    private:
        Mat3 rotation;
        Vec3 translation;
        // Direction of travel of light rays
        Vec3 dir = {0.0f, 0.0f, 1.0f};
        // This is the intensity if direct light from source (white light so only need 1 channel to represent it)
        float diffuse = 1.0f;
        // This is intensity of indirect light that bounces off other obj in scene (white light so only need 1 channel
        // to represent it)
        float ambient = 0.15f;
    };

    // Texture clamped ps with light intensity input
    class PixelShader {
    public:
        PixelShader() : pTex(), tex_width(), tex_height(), tex_xclamp(), tex_yclamp() {
        }

        template<class Input>
        Color operator()(const Input& in) const {
            // Lookup color in texture
            const Vec3 color = Vec3(pTex->GetPixel((unsigned int)std::min(in.t[0] * tex_width + 0.5f, tex_xclamp),
                (unsigned int)std::min(in.t[1] * tex_height + 0.5f, tex_yclamp)));

            // Use texture color as material to determine ratio / magnitude of the different color components diffuse
            // reflected from triangle at this pt.
            return Color(color * in.l);
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
