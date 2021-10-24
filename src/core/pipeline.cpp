#include <core/pipeline.hpp>

namespace SyncX {

Matrix4f VertexShader::GetModelMatrix(const Vector3f& translation, const Vector3f& axis, float radian) {
    return Matrix4f();
}

Matrix4f VertexShader::GetViewMatrix(const Vector3f& pos, const Vector3f& top, const Vector3f& dir) {
    return Matrix4f();
}

Matrix4f VertexShader::GetProjectionMatrix(float fov, float aspect, float near, float far) {
    return Matrix4f();
}

void VertexShader::operator()(Scene* sc, VertexShaderPayload& payload) {
    // std::vector<Vertex>& vertex_buffer = sc->m_Vertrices;
    
    // Need a temporary buffer for transformed vertrices
    // or call it stream output?
    //for (Vertex vertex : sc->m_Vertrices) {
    //    // for each member;
    //    m_VertexBuffer.push_back(vertex);
    //}    
}

}   // namespace SyncX