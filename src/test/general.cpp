#include <iostream>
#include <cstring>
#include <vector>

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



int main(int argc, char** argv) {


    // std::vector<float> v;
    // for (int i = 0; i <= 255; ++i) {
    //     v.push_back(float(i) / 255);
    // }
    
    // std::cout << std::endl;

    // for (const auto& i : v) {
    //     std::cout << i * 255 << ' ';
    // }

    uint8_t b1 = 255, b2 = 1;
    uint8_t r = b1 + b2;
    std::cout << (int)r;

    return 0;
}