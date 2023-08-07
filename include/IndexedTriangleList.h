#pragma once

#include "LinearAlgebra/Vector3.h"

#include "tiny_obj_loader.h"
#include "Miniball.h"

#include <vector>
#include <fstream>
#include <cctype>


template<class T>
struct IndexedTriangleList {
public:
    IndexedTriangleList() = default;

    IndexedTriangleList(std::vector<T> verts_in, std::vector<size_t> indices_in)
        : vertices(std::move(verts_in)), indices(std::move(indices_in)) {
        assert(vertices.size() > 2);
        assert(indices.size() % 3 == 0);
    }

    static IndexedTriangleList<T> Load(const std::string& filename) {
        IndexedTriangleList<T> tl;

        // Check first line of file to see if CCW winding comment exists
        bool isCCW = false;
        {
            std::ifstream file(filename);
            std::string firstline;
            std::getline(file, firstline);
            std::transform(firstline.begin(), firstline.end(), firstline.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            if (firstline.find("ccw") != std::string::npos) {
                isCCW = true;
            }
        }

        // These will be filled by obj loading function
        using namespace tinyobj;

        attrib_t attrib;
        std::vector<shape_t> shapes;
        std::string err;

        // Load/parse the obj file
        const bool ret = LoadObj(&attrib, &shapes, nullptr, &err, filename.c_str());

        // Check for errors
        if (!err.empty() && err.substr(0, 4) != "WARN") {
            throw std::runtime_error(("LoadObj returned error:" + err + " File:" + filename).c_str());
        }

        if (!ret) {
            throw std::runtime_error(("LoadObj returned false  File:" + filename).c_str());
        }

        if (shapes.size() == 0u) {
            throw std::runtime_error(("LoadObj object file had no shapes  File:" + filename).c_str());
        }

        // Extract vertex data
        // `attrib.vertices` is a flat std::vector of floats corresponding to vertex positions, laid out as xyzxyzxyz...
        // etc. first preallocate required space in OUR std::vector
        tl.vertices.reserve(attrib.vertices.size() / 3u);

        // Iterate over individual vertices, construct Vec3s in OUR vector
        for (int i = 0; i < attrib.vertices.size(); i += 3) {
            tl.vertices.emplace_back(Vec3 {attrib.vertices[i + 0], attrib.vertices[i + 1], attrib.vertices[i + 2]});
        }

        // Extract index data
        // Obj file can contain multiple meshes, we assume just 1
        const auto& mesh = shapes[0].mesh;
        // Mesh contains a `std::vector` of `num_face_vertices` (uchar) and a flat `std::vector` of indices. If all
        // faces are triangles then for any face `f`, the first index of that faces is `[f * 3n]`
        tl.indices.reserve(mesh.indices.size());

        for (size_t f = 0; f < mesh.num_face_vertices.size(); f++) {
            // Make sure there are no non-triangle faces
            if (mesh.num_face_vertices[f] != 3u) {
                std::stringstream ss;
                ss << "LoadObj error face #" << f << " has " << mesh.num_face_vertices[f] << " vertices";

                throw std::runtime_error(ss.str().c_str());
            }

            // Load set of 3 indices for each face into OUR index std::vector
            for (size_t vn = 0; vn < 3u; vn++) {
                const auto idx = mesh.indices[f * 3u + vn];
                tl.indices.push_back(size_t(idx.vertex_index));
            }

            // Reverse winding if file marked as CCW
            if (isCCW) {
                // Swapping any two indices reverse the winding dir of triangle
                std::swap(tl.indices.back(), *std::prev(tl.indices.end(), 2));
            }
        }

        return tl;
    }

    void AdjustToTrueCenter() {
        // Used to enable miniball to access vertex pos info
        struct VertexAccessor {
            // Iterator type for iterating over vertices
            typedef std::vector<T>::const_iterator Pit;
            // Type for iterating over components of vertex (pointer is used to iterate over members of class here)
            typedef const float* Cit;

            // Functor that miniball uses to get element iter based on vertex iter
            Cit operator()(Pit it) const {
                return &it->pos[0];
            }
        };

        // Solve the minimum bounding sphere
        Miniball::Miniball<VertexAccessor> mb(3, vertices.cbegin(), vertices.cend());
        // Get center of min sphere
        // Result is a pointer to float[3] (what a shitty fucking interface)
        const auto pc = mb.center();
        const Vec3 center = {*pc, *std::next(pc), *std::next(pc, 2)};

        // Adjust all vertices so that center of minimal sphere is at 0,0
        for (auto& v : vertices) {
            v.pos -= center;
        }
    }

    float GetRadius() const {
        // Find element with max distance from 0,0; that is our radius
        return std::max_element(vertices.begin(), vertices.end(), [](const T& v0, const T& v1) {
            return v0.pos.GetMagnitudeSquared() < v1.pos.GetMagnitudeSquared();
        })->pos.GetMagnitude();
    }

    std::vector<T> vertices;
    std::vector<size_t> indices;
};
