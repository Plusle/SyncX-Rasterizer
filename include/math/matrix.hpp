#pragma once 

#include <math/vector.hpp>

#include <ostream>
#include <cstring>

namespace SyncX {

    enum class MatrixMajor { ROW, COL };

    template <typename T>
    struct Matrix2 {};

    template <typename T>
    struct Matrix3 {
        constexpr static int m_Size = 3 * 3;
        constexpr static int m_Bytes = m_Size * sizeof(T);

        Matrix3() { std::memset(this, 0, m_Bytes); }
        Matrix3(const Matrix3& m) { std::memcpy(this, &m, m_Bytes); }
        Matrix3(const Vector3<T>& v1, const Vector3<T>& v2, const Vector3<T>& v3,
#if   defined(MATRIX_ROW_MAJOR) || defined(MATRIX_COL_MAJOR)
    #ifdef MATRIX_ROW_MAJOR
                MatrixMajor major = MatrixMajor::ROW);
    #else
                MatrixMajor major = MatrixMajor::COL);
    #endif
#else 
                MatrixMajor major = MatrixMajor::ROW);
#endif

        Matrix3& operator=(const Matrix3& mat);
        bool operator==(const Matrix3& mat) const;
        Matrix3 operator+(const Matrix3& mat) const;
        Matrix3& operator+=(const Matrix3& mat);
        Matrix3 operator-() const;
        Matrix3 operator-(const Matrix3& mat) const;
        Matrix3& operator-=(const Matrix3& mat);
        Matrix3 operator*(const Matrix3& mat) const;
        Matrix3 operator*(T c) const;
        Matrix3& operator*=(T c);
        Matrix3 operator/(T c) const;
        Matrix3& operator/=(T c);
        T* operator[](size_t i);
        const T* operator[](size_t i) const;

        T m_Elem[m_Size];
    };

    template <typename T>
    inline Matrix3<T> CoefficentWiseProduct(const Matrix3<T>& a, const Matrix3<T>& b);

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Matrix3<T>& mat);

    template <typename T>
    struct Matrix4 {
        constexpr static int m_Size = 4 * 4;
        constexpr static int m_Bytes = sizeof(T) * m_Size;

        Matrix4() { std::memset(this, 0, m_Bytes); }
        Matrix4(const Matrix4& m) { std::memcpy(this, &m, m_Bytes); }
        Matrix4(const Vector4<T>& v1, const Vector4<T>& v2, const Vector4<T>& v3, const Vector4<T>& v4, 
#if   defined(MATRIX_ROW_MAJOR)
                MatrixMajor major = MatrixMajor::ROW);
#elif defined(MATRIX_COL_MAJOR)
                MatrixMajor major = MatrixMajor::COL);
#else 
                MatrixMajor major = MatrixMajor::ROW);
#endif

        Matrix4& operator=(const Matrix4& mat);
        bool operator==(const Matrix4& mat) const;
        Matrix4 operator+(const Matrix4& mat) const;
        Matrix4& operator+=(const Matrix4& mat);
        Matrix4 operator-() const;
        Matrix4 operator-(const Matrix4& mat) const;
        Matrix4& operator-=(const Matrix4& mat);
        Matrix4 operator*(const Matrix4& mat) const;
        Matrix4 operator*(T c) const;
        Matrix4& operator*=(T c);
        Matrix4 operator/(T c) const;
        Matrix4& operator/=(T c);
        T* operator[](size_t i);
        const T* operator[](size_t i) const;

        T m_Elem[m_Size];
    };

    template <typename T>
    inline Matrix4<T> CoefficentWiseProduct(const Matrix4<T>& a, const Matrix4<T>& b);

    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Matrix4<T>& mat);

    using Matrix3f = Matrix3<float>;

    using Matrix4f = Matrix4<float>;

}   // namespace SyncX

#include "matrix.inl"