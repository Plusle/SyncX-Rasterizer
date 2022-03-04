#include <cmath>
#include <cassert>

namespace SyncX {

// Vector2 ---------------------------------------------------------------------

template <typename T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& v) {
    this->x = v.x; this->y = v.y;
    return *this;
}

template <typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& v) const {
    return { this->x + v.x, this->y + v.y };
}

template <typename T>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& v) {
    this->x += v.x; this->y += v.y;
    return *this;
}

template <typename T>
Vector2<T> Vector2<T>::operator-() const {
    return { -this->x, -this->y };
}

template <typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& v) const {
    return { this->x - v.x, this->y - v.y }; 
}

template <typename T>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& v) {
    this->x -= v.x; this->y -= v.y;
    return *this;
}

template <typename T>
Vector2<T> Vector2<T>::operator*(const Vector2& v) const {
    return { this->x * v.x, this->y * v.y };
}

template <typename T>
Vector2<T> Vector2<T>::operator*(T c) const {
    return { c * this->x, c * this->y };
}

template <typename T>
Vector2<T>& Vector2<T>::operator*=(T c) {
    this->x *= c; this->y *= c;
    return *this;
}

template <typename T>
Vector2<T> Vector2<T>::operator/(T c) const {
    assert(c != 0);
    return { this->x / c, this->y / c };
}

template <typename T>
Vector2<T>& Vector2<T>::operator/=(T c) {
    assert(c != 0);
    this->x /= c; this->y /= c;
    return *this;
}

template <typename T>
T& Vector2<T>::operator[](size_t i) {
    switch (i) {
        case 0:
            return x;
        case 1:
            return y;
    }
    throw std::runtime_error("Unexpected index for Vector2");
}

template <typename T>
const T& Vector2<T>::operator[](size_t i) const {
    switch (i) {
        case 0:
            return x;
        case 1:
            return y;
    }
    throw std::runtime_error("Unexpected index for Vector2");
}

template <typename T>
T Vector2<T>::SquaredLength() const {
    return x * x + y * y;
}

template <typename T>
float Vector2<T>::Length() const {
    return std::sqrt(this->SquaredLength());
}

template <typename T>
Vector2<T>& Vector2<T>::Normalized() {
    return *this /= this->Length();
}

template <typename T>
Vector2<T> Vector2<T>::TempNormalized() {
    return *this / this->Length();
}

template <typename T>
inline T dot(const Vector2<T>& a, const Vector2<T>& b) {
    return a.x * b.x + a.y * b.y;
}

template <typename T>
inline Vector2<T> operator*(T c, const Vector2<T>& v) {
    return v * c;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
    os << "[" << v.x << ", " << v.y << "]\n";
    return os;
}

// Vector3 ---------------------------------------------------------------------

template <typename T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& v) {
    this->x = v.x; this->y = v.y; this->z = v.z;
    return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) const {
    return { this->x + v.x, this->y + v.y, this->z + v.z };
}

template <typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& v) {
    this->x += v.x; this->y += v.y; this->z += v.z;
    return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator-() const {
    return { -this->x, -this->y, -this->z };
}

template <typename T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& v) const {
    return { this->x - v.x, this->y - v.y, this->z - v.z }; 
}

template <typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& v) {
    this->x -= v.x; this->y -= v.y; this->z -= v.z;
    return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator*(const Vector3& v) const {
    return { this->x * v.x, this->y * v.y, this->z * v.z };
}

template <typename T>
Vector3<T> Vector3<T>::operator*(T c) const {
    return { c * this->x, c * this->y, c * this->z };
}

template <typename T>
Vector3<T>& Vector3<T>::operator*=(T c) {
    this->x *= c; this->y *= c; this->z *= c;
    return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator/(T c) const {
    assert(c != 0);
    return { this->x / c, this->y / c, this->z / c };
}

template <typename T>
Vector3<T>& Vector3<T>::operator/=(T c) {
    assert(c != 0);
    this->x /= c; this->y /= c; this->z /= c;
    return *this;
}

template <typename T>
T& Vector3<T>::operator[](size_t i) {
    switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
    }
    throw std::runtime_error("Unexpected index for Vector3");
}

template <typename T>
const T& Vector3<T>::operator[](size_t i) const {
    switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
    }
    throw std::runtime_error("Unexpected index for Vector3");
}

template <typename T>
T Vector3<T>::SquaredLength() const {
    return x * x + y * y + z * z;
}

template <typename T>
float Vector3<T>::Length() const {
    return std::sqrt(this->SquaredLength());
}

template <typename T>
Vector3<T>& Vector3<T>::Normalized() {
    return *this /= this->Length();
}

template <typename T>
Vector3<T> Vector3<T>::TempNormalized() {
    return *this / this->Length();
}

template <typename T>
Vector2<T> Vector3<T>::toVec2() const {
    return Vector2<T>(this->x, this->y);
}

template <typename T>
inline T dot(const Vector3<T>& a, const Vector3<T>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
inline Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b) {
    T x = a[1] * b[2] - a[2] * b[1];
    T y = a[2] * b[0] - a[0] * b[2];
    T z = a[0] * b[1] - a[1] * b[0];
    return Vector3<T>(x, y, z);
}

template <typename T>
inline Vector3<T> operator*(T c, const Vector3<T>& v) {
    return v * c;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
    os << "[" << v.x << ", " << v.y << ", " << v.z << "]\n";
    return os;
}

// Vector4<T> ---------------------------------------------------------------------

template <typename T>
Vector4<T>& Vector4<T>::operator=(const Vector4<T>& v) {
    this->x = v.x; this->y = v.y; this->z = v.z; this->w = v.w;
    return *this;
}
    
template <typename T>
Vector4<T> Vector4<T>::operator+(const Vector4<T>& v) const {
    return { this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w };
}
    
template <typename T>
Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& v) {
    this->x += v.x; this->y += v.y; this->z += v.z; this->w += v.w;
    return *this;
}
    
template <typename T>
Vector4<T> Vector4<T>::operator-() const {
    return { -this->x, -this->y, -this->z, -this->w };
}
    
template <typename T>
Vector4<T> Vector4<T>::operator-(const Vector4<T>& v) const {
    return { this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w };
}
    
template <typename T>
Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& v) {
    this->x -= v.x; this->y -= v.y; this->z -= v.z; this->w -= v.w;
    return *this;
}
    
template <typename T>
Vector4<T> Vector4<T>::operator*(const Vector4<T>& v) const {
    return { this->x * v.x, this->y * v.y, this->z * v.z, this->w * v.w };
}
    
template <typename T>
Vector4<T> Vector4<T>::operator*(T c) const {
    return { this->x * c, this->y * c, this->z * c, this->w * c };
}
    
template <typename T>
Vector4<T>& Vector4<T>::operator*=(T c) {
    this->x *= c; this->y *= c; this->z *= c; this->w *= c;
    return *this;
}
    
template <typename T>
Vector4<T> Vector4<T>::operator/(T c) const {
    assert(c != 0.0 && c != -0.0);
    return { this->x / c, this->y / c, this->z / c, this->w / c };
}
    
template <typename T>
Vector4<T>& Vector4<T>::operator/=(T c) {
    assert(c != 0.0 && c != -0.0);
    this->x /= c; this->y /= c; this->z /= c; this->w /= c;
    return *this;
}
    
template <typename T>
T& Vector4<T>::operator[](size_t i) {
    switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
    }
    throw std::runtime_error("Unexpected index for Vector4");
}

template <typename T>
const T& Vector4<T>::operator[](size_t i) const {
    switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
    }
    throw std::runtime_error("Unexpected index for Vector4");
}

template <typename T>
T Vector4<T>::SquaredLength() const {
    return x * x + y * y + z * z + w * w;
}
    
template <typename T>
float Vector4<T>::Length() const {
    return std::sqrt(this->SquaredLength());
}
    
template <typename T>
Vector4<T>& Vector4<T>::Normalized() {
    return *this /= this->Length();
}
    
template <typename T>
Vector4<T> Vector4<T>::TempNormalized() {
    return *this / this->Length();
}

template <typename T>
Vector2<T> Vector4<T>::toVec2() const {
    return Vector2<T>(this->x, this->y);
}

template <typename T>
Vector3<T> Vector4<T>::toVec3() const {
    return Vector3<T>(this->x, this->y, this->z);
}

template <typename T>
inline T dot(const Vector4<T>& a, const Vector4<T>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T>
inline Vector4<T> operator*(T c, const Vector4<T>& v) {
    return v * c;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector4<T>& v) {
    os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]\n";
    return os;
}

}