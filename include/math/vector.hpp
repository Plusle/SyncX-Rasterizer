#pragma once

#include <cstdint>
#include <ostream>

namespace SyncX {

    template <typename T>
    struct Vector2 {
        Vector2() : x(0), y(0) {}
        Vector2(T xx, T yy) : x(xx), y(yy) {}
        Vector2(const Vector2& v) : x(v.x), y(v.y) {}

        Vector2& operator=(const Vector2&);
        Vector2 operator+(const Vector2&) const;
        Vector2& operator+=(const Vector2&);
        Vector2 operator-() const;
        Vector2 operator-(const Vector2&) const;
        Vector2& operator-=(const Vector2&);
        Vector2 operator*(const Vector2&) const;
        Vector2 operator*(T c) const;
        Vector2& operator*=(T c);
        Vector2 operator/(T c) const;
        Vector2& operator/=(T c);
        T& operator[](size_t);
        const T& operator[](size_t) const;

        T SquaredLength() const;
        float Length() const;
        Vector2& Normalized();
        Vector2 TempNormalized();

        T x, y;
    };

    template <typename T>
    inline T dot(const Vector2<T>& a, const Vector2<T>& b);
    template <typename T>
    inline Vector2<T> operator*(T c, const Vector2<T>& v);
    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vector2<T>& v);

    template <typename T>
    struct Vector3 {
        Vector3() : x(0), y(0), z(0) {}
        Vector3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
        Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

        Vector3& operator=(const Vector3&);
        Vector3 operator+(const Vector3&) const;
        Vector3& operator+=(const Vector3&);
        Vector3 operator-() const;
        Vector3 operator-(const Vector3&) const;
        Vector3& operator-=(const Vector3&);
        Vector3 operator*(const Vector3&) const;
        Vector3 operator*(T c) const;
        Vector3& operator*=(T c);
        Vector3 operator/(T c) const;
        Vector3& operator/=(T c);
        T& operator[](size_t);
        const T& operator[](size_t) const;

        T SquaredLength() const;
        float Length() const;
        Vector3& Normalized();
        Vector3 TempNormalized();

        Vector2<T> toVec2() const;

        T x, y, z;
    };

    template <typename T>
    inline T dot(const Vector3<T>& a, const Vector3<T>& b);
    template <typename T>
    inline Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b);
    template <typename T>
    inline Vector3<T> operator*(T c, const Vector3<T>& v);
    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vector3<T>& v);


    template <typename T>
    struct Vector4 {
        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(T xx, T yy, T zz, T ww) : x(xx), y(yy), z(zz), w(ww) {}
        Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

        Vector4& operator=(const Vector4&);
        Vector4 operator+(const Vector4&) const;
        Vector4& operator+=(const Vector4&);
        Vector4 operator-() const;
        Vector4 operator-(const Vector4&) const;
        Vector4& operator-=(const Vector4&);
        Vector4 operator*(const Vector4&) const;
        Vector4 operator*(T c) const;
        Vector4& operator*=(T c);
        Vector4 operator/(T c) const;
        Vector4& operator/=(T c);
        T& operator[](size_t);
        const T& operator[](size_t) const;

        T SquaredLength() const;
        float Length() const;
        Vector4& Normalized();
        Vector4 TempNormalized();

        Vector2<T> toVec2() const;
        Vector3<T> toVec3() const;

        T x, y, z, w;
    };


    template <typename T>
    inline T dot(const Vector4<T>& a, const Vector4<T>& b);
    template <typename T>
    inline Vector4<T> operator*(T c, const Vector4<T>& v);
    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const Vector4<T>& v);

    using Vector2f = Vector2<float>;
    using Vector2i = Vector2<int32_t>;

    using Vector3f = Vector3<float>;
    using Vector3i = Vector3<int32_t>;

    using Vector4f = Vector4<float>;
    using Vector4i = Vector4<int32_t>;

};

#include "vector.inl"