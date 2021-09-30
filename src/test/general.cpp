#include <iostream>
#include <cstring>

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

void fun(int (&i)[3]) {

}

// void fun(int* i) {
    
// }

int main(int argc, char** argv) {
    // vec v1{1, 2};
    // vec v2{4, 5};
    // mat m(v1, v2);
    // std::cout << m.c[0] << " " << m.c[1] << "\n" << m.c[2] << " " << m.c[3]; 

#define TEST2
#if defined(TEST1)
    std::cout << "test1";
#elif defined(TEST2)
    std::cout << "test2";
#else
    std::cout << "default";
#endif

    int i[3] = { 1, 2, 3 };
    fun(i);

    return 0;
}