#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

#include <math/linear.hpp>

struct vec {
    float x, y;

    float& operator[](size_t i) {
        return i == 0 ? x : y;
    }
};

struct mat {
    mat(const vec& v1, const vec& v2) {
        std::memcpy(&c[0], &(v1.x), sizeof(vec));
        std::memcpy(&c[0], &(v2.x), sizeof(vec));
    }



    float c[4];
};


struct array {
    array() : v({ nullptr, nullptr, nullptr }) {}

    void* v[3];
};


int main(int argc, char** argv) {


    // std::vector<float> v;
    // for (int i = 0; i <= 255; ++i) {
    //     v.push_back(float(i) / 255);
    // }
    
    // std::cout << std::endl;

    // for (const auto& i : v) {
    //     std::cout << i * 255 << ' ';
    // }

    // uint8_t b1 = 255, b2 = 1;
    // uint8_t r = b1 + b2;
    // std::cout << (int)r;
    // const char* word = "Hello, world!";
    // const char* a = &word[std::strlen(word) - 1];
    // std::cout << a;


    //std::vector<int> src{1, 3, 5, 7, 9};
    //std::vector<int> dst;
    //std::copy(src.cbegin(), src.cend(), std::back_inserter(dst));
    //for (auto i : dst) {
    //    std::cout << i << ' ';
    //}
    //std::cout << std::endl;

    //std::vector<int> dst2;
    //std::transform(src.cbegin(), src.cend(), std::back_inserter(dst2),
    //    [](int i) -> int { return i + 1; });
    //for (auto i : dst2) {
    //    std::cout << i << ' ';
    //}
    //std::cout << std::endl;

    //auto mat = SyncX::MakeTranslateMatrix(SyncX::Vector3f(1.0, 2.0, 3.0));
    //for (int i = 0; i < 4; ++i) {
    //    for (int j = 0; j < 4; ++j)
    //        std::cout << mat[i][j] << ' ';
    //    std::cout << std::endl;
    //}

    //auto rot = SyncX::MakeRotateMatrix(SyncX::Vector3f(0.0, 1.0, 0.0), 3.1415926);
    //std::cout << rot;

    //std::cout << mat * rot;

    //auto model = SyncX::MakeModelMatrix(SyncX::Vector3f(0.0, 1.0, 0.0), 3.1415926, SyncX::Vector3f(1.0, 2.0, 3.0));

    //std::cout << model;

    //SyncX::Vector3f v(1, 2, 3);
    //for (int i = 0; i < 3; ++i) {
    //    std::cout << v[i] << ' ';
    //}

    return 0;
}