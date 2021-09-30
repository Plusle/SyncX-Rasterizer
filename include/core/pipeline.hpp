#pragma once

#include <math/matrix.hpp>
#include <core/scene.hpp>

namespace SyncX {

struct VertexShaderPayload {
    Matrix4f model;
    Matrix4f view;
    Matrix4f projection;
};

struct FragmentShaderPayload {
    
};

struct PipelineProcess { 
    virtual std::vector<Vertex>& StreamOutput() = 0;
};

struct VertexShader {
    static Matrix4f GetModelMatrix(const Vector3f& translation, const Vector3f& axis, float radian);
    static Matrix4f GetViewMatrix(const Vector3f& pos, const Vector3f& top, const Vector3f& dir);
    static Matrix4f GetProjectionMatrix(float fov, float aspect, float near, float far);
    void operator()(Scene* sc, VertexShaderPayload& payload);
    std::vector<Vertex>& StreamOut();

    std::vector<Vertex> m_VertexBuffer;
};

struct Rasterizer {
    void operator()(Scene* sc) const;
};

struct FragmentShader {

    void operator()(Scene* sc, FragmentShaderPayload& payload) const;
};

}   // namespace SyncX