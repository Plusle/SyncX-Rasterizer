#include <cassert>

namespace SyncX {
template <typename T>
Matrix3<T>::Matrix3(const Vector3<T>& v1, const Vector3<T>& v2, const Vector3<T>& v3) {
    std::memcpy(&m_Elem[0], &v1[0], sizeof(Vector3<T>));
    std::memcpy(&m_Elem[3], &v2[0], sizeof(Vector3<T>));
    std::memcpy(&m_Elem[6], &v3[0], sizeof(Vector3<T>));
}

template <typename T>
Matrix3<T> Matrix3<T>::Identity() {
    return Matrix3<T>(Vector3<T>(1, 0, 0), Vector3<T>(0, 1, 0), Vector3<T>(0, 0, 1));
}

template <typename T>
Matrix3<T>& Matrix3<T>::operator=(const Matrix3<T>& mat) {
    std::memcpy(this, &mat, sizeof(Matrix3<T>));
    return *this;
}
    
template <typename T>
bool Matrix3<T>::operator==(const Matrix3<T>& mat) const {
    bool eq = true;
    for (auto i = 0; i < m_Size; ++i) {
        eq &= m_Elem[i] == mat.m_Elem[i];
    }
    return eq;
}
    
template <typename T>
Matrix3<T> Matrix3<T>::operator+(const Matrix3<T>& mat) const {
    Matrix3<T> tmp(*this);
    tmp += mat;
    return tmp;
}
    
template <typename T>
Matrix3<T>& Matrix3<T>::operator+=(const Matrix3<T>& mat) {
    for (auto i = 0; i < m_Size; ++i) {
        this->m_Elem[i] += mat.m_Elem[i];
    }
    return *this;
}
    
template <typename T>
Matrix3<T> Matrix3<T>::operator-() const {
    Matrix3<T> tmp;
    for (auto i = 0; i < m_Size; ++i) {
        tmp.m_Elem[i] = -this->m_Elem[i];
    }
    return tmp;
}
    
template <typename T>
Matrix3<T> Matrix3<T>::operator-(const Matrix3<T>& mat) const {
    Matrix3<T> tmp(*this);
    tmp -= mat;
    return tmp;
}
    
template <typename T>
Matrix3<T>& Matrix3<T>::operator-=(const Matrix3<T>& mat) {
    for (auto i = 0; i < m_Size; ++i) {
        this->m_Elem[i] -= mat.m_Elem[i];
    }
    return *this;
}
    
template <typename T>
Matrix3<T> Matrix3<T>::operator*(const Matrix3<T>& mat) const {
    Matrix3<T> result;
    for (int i = 0; i < 3; ++i) 
        for (int j = 0; j < 3; ++j) 
            for (int k = 0; k < 3; ++k) 
                result[i][j] += (*this)[i][k] * mat[k][j];
    return result;
}
    
template <typename T>
Matrix3<T> Matrix3<T>::operator*(T c) const {
    Matrix3<T> tmp(*this);
    tmp *= c;
    return tmp;
}
    
template <typename T>
Matrix3<T>& Matrix3<T>::operator*=(T c) {
    for (auto i = 0; i < m_Size; ++i) {
        this->m_Elem[i] *= c;
    }
    return *this;
}

template <typename T>
Vector3<T> Matrix3<T>::operator*(const Vector3<T>& v) const {    
    T a = 0, b = 0, c = 0;
    for (auto i = 0; i < 3; ++i) {
        a += (*this)[0][i] * v[i];
        b += (*this)[1][i] * v[i];
        c += (*this)[2][i] * v[i];
    }
    return Vector3<T>(a, b, c);
}
    
template <typename T>
Matrix3<T> Matrix3<T>::operator/(T c) const {
    assert(c != 0.0 && c != -0.0);
    Matrix3<T> tmp(*this);
    tmp /= c;
    return tmp;
}
    
template <typename T>
Matrix3<T>& Matrix3<T>::operator/=(T c) {
    assert(c != 0.0 && c != -0.0);
    for (auto i = 0; i < m_Size; ++i) {
        this->m_Elem[i] /= c;
    }
    return *this;
}

template <typename T>
T* Matrix3<T>::operator[](size_t i) {
    return &m_Elem[i * 3];
}
    
template <typename T>
const T* Matrix3<T>::operator[](size_t i) const {
    return &m_Elem[i * 3];
}

template <typename T>
inline Matrix3<T> CoefficentWiseProduct(const Matrix3<T>& a, const Matrix3<T>& b) {
    Matrix3<T> result(a);
    for (int i = 0; i < Matrix3<T>::m_Size; ++i) {
        result.m_Elem[i] *= b.m_Elem[i];
    }
    return result;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Matrix3<T>& mat) {
    for (auto i = 0; i < 3; ++i) {
        os << '[';
        for (auto j = 0; j < 3; ++j) {
            if (j == 2) os << mat.m_Elem[i * 3 + j];
            else os << mat.m_Elem[i * 3 + j] << '\t';
        }
        os << "]\n";
    }
    os << "\n";
    return os;
}

template <typename T>
inline Matrix3<T> MakeMatrix3from4(const Matrix4<T>& m) {
    Matrix3<T> res;
    for (auto i = 0; i < 3; ++i) {
        for (auto j = 0; j < 3; ++j) {
            res[i][j] = m[i][j];
        }
    }
    return res;
}

template <typename T>
inline Matrix3<T> MakeMinor(const Matrix4<T>& m, int i, int j) {
    int index = 0;
    Matrix3<T> res;
    for (auto x = 0; x < 4; ++x) {
        if (x == i) continue;
        for (auto y = 0; y < 4; ++y) {
            if (y == j) continue;
            res.m_Elem[index++] = m[x][y];
        }
    }
    return res;
}

template <typename T>
inline float Determinant(const Matrix3<T>& m) {
    float det = 0;
    det += m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
    det -= m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
    det += m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    return det;    
}

template <typename T>
Matrix4<T>::Matrix4(const Vector4<T>& v1, const Vector4<T>& v2, const Vector4<T>& v3, const Vector4<T>& v4) {
    std::memcpy(&m_Elem[0],  &(v1.x), sizeof(Vector4<T>));
    std::memcpy(&m_Elem[4],  &(v2.x), sizeof(Vector4<T>));
    std::memcpy(&m_Elem[8],  &(v3.x), sizeof(Vector4<T>));
    std::memcpy(&m_Elem[12], &(v4.x), sizeof(Vector4<T>));
}

template <typename T>
Matrix4<T> Matrix4<T>::Identity() {
    return Matrix4<T>(Vector4<T>(1, 0, 0, 0), Vector4<T>(0, 1, 0, 0), Vector4<T>(0, 0, 1, 0), Vector4<T>(0, 0, 0, 1));
}
    
template <typename T>
Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& mat) {
    std::memcpy(this, &mat, m_Bytes);
    return *this;
}
    
template <typename T>
bool Matrix4<T>::operator==(const Matrix4<T>& mat) const {
    bool eq = true;
    for (auto i = 0; i < m_Size; ++i) {
        eq &= this->m_Elem[i] == mat.m_Elem[i];
    }
    return eq;
}
    
template <typename T>
Matrix4<T> Matrix4<T>::operator+(const Matrix4<T>& mat) const {
    Matrix4<T> tmp(*this);
    tmp += mat;
    return tmp;
}
        
template <typename T>
Matrix4<T>& Matrix4<T>::operator+=(const Matrix4<T>& mat) {
    for (auto i = 0; i < m_Size; ++i) {
        this->m_Elem[i] += mat.m_Elem[i];
    }
    return *this;
}
    
template <typename T>
Matrix4<T> Matrix4<T>::operator-() const {
    Matrix4<T> tmp;
    for (auto i = 0; i < m_Size; ++i) {
        tmp.m_Elem[i] = -this->m_Elem[i];
    }
    return tmp;
}
    
template <typename T>
Matrix4<T> Matrix4<T>::operator-(const Matrix4<T>& mat) const {
    Matrix4<T> tmp(*this);
    tmp -= mat;
    return tmp;
}
    
template <typename T>
Matrix4<T>& Matrix4<T>::operator-=(const Matrix4<T>& mat) {
    for (auto i = 0; i < m_Size; ++i) {
        this->m_Elem[i] -= mat.m_Elem[i];
    }
    return *this;
}
    
template <typename T>
Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& mat) const {
    Matrix4<T> result;
    for (int i = 0; i < 4; ++i) 
        for (int j = 0; j < 4; ++j) 
            for (int k = 0; k < 4; ++k) 
                result[i][j] += (*this)[i][k] * mat[k][j];
    return result;
}
    
template <typename T>
Matrix4<T> Matrix4<T>::operator*(T c) const {
    Matrix4<T> tmp(*this);
    tmp *= c;
    return tmp;
}
    
template <typename T>
Matrix4<T>& Matrix4<T>::operator*=(T c) {
    for (auto i = 0; i < m_Size; ++i) {
        this->m_Elem[i] *= c;
    }
    return *this;
}

template <typename T>
Vector4<T> Matrix4<T>::operator*(const Vector4<T>& v) const {
    T a = 0, b = 0, c = 0, d = 0;
    for (auto i = 0; i < 4; ++i) {
        a += (*this)[0][i] * v[i];
        b += (*this)[1][i] * v[i];
        c += (*this)[2][i] * v[i];
        d += (*this)[3][i] * v[i];
    }
    return Vector4<T>(a, b, c, d);
}
    
template <typename T>
Matrix4<T> Matrix4<T>::operator/(T c) const {
    assert(c != 0.0 && c != -0.0);
    Matrix4<T> tmp(*this);
    tmp /= c;
    return tmp;
}
    
template <typename T>
Matrix4<T>& Matrix4<T>::operator/=(T c) {
    assert(c != 0.0 && c != -0.0);
    for (auto i = 0; i < m_Size; ++i) {
        this->m_Elem[i] /= c;
    }
    return *this;
}
    
template <typename T>
T* Matrix4<T>::operator[](size_t i) {
    return &m_Elem[i * 4];
}
    
template <typename T>
const T* Matrix4<T>::operator[](size_t i) const {
    return &m_Elem[i * 4];
}

template <typename T>
inline Matrix4<T> CoefficentWiseProduct(const Matrix4<T>& a, const Matrix4<T>& b) {
    Matrix4<T> result(a);
    for (int i = 0; i < Matrix4<T>::m_Size; ++i) {
        result.m_Elem[i] *= b.m_Elem[i];
    }
    return result;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Matrix4<T>& mat) {
    for (auto i = 0; i < 4; ++i) {
        os << '[';
        for (auto j = 0; j < 4; ++j) {
            if (j == 3) os << mat.m_Elem[i * 4 + j];
            else os << mat.m_Elem[i * 4 + j] << '\t';
        }
        os << "]\n";
    }
    os << "\n";
    return os;
}

template <typename T>
inline float Determinant(const Matrix4<T>& m) {
    float det = 0;
    det += m[0][0] * Determinant(MakeMinor(m, 0, 0));
    det -= m[0][1] * Determinant(MakeMinor(m, 0, 1));
    det += m[0][2] * Determinant(MakeMinor(m, 0, 2));
    det -= m[0][3] * Determinant(MakeMinor(m, 0, 3));
    return det;
}

template <typename T>
inline Matrix4<T> MakeInverse(const Matrix4<T>& m) {
    Matrix4<T> inverse;

    float det = Determinant(m);
    if (det == 0.0f) {
        std::cerr << "Singluar matrix4 is detected:\n" << m << std::endl;
        std::exit(1);
    }
    det = 1 / det;

    auto sign = [](int i, int j) -> int { return (i + j) % 2 == 0 ? 1 : -1; };
    for (auto i = 0; i < 4; ++i) {
        for (auto j = 0; j < 4; ++j) {
            inverse[i][j] = sign(i, j) * Determinant(MakeMinor(m, i, j));
        }
    }
    return MakeTranspose(inverse * det);
}

template <typename T>
inline Matrix4<T> MakeTranspose(const Matrix4<T>& m) {
    Matrix4<T> transpose;
    for (auto i = 0; i < 4; ++i) {
        for (auto j = 0; j < 4; ++j) {
            transpose[i][j] = m[j][i];
        }
    }
    return transpose;
}

}   // namespace SyncX