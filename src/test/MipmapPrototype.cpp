#define STB_IMAGE_IMPLEMENTATION
#include <external/stb_image.h>

#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

int main(int argc, char** argv) {
    int32_t x, y, ch, req;
    std::string filename = argc > 1 ? argv[1] : "texture.png";
    std::string extension = filename.substr(filename.length() - 3, 3);
    const char* ext = extension.c_str();

    std::cout << ext << std::endl;
    req = !strcmp(ext, "png") ? 4 : 3;
    uint8_t* origin = stbi_load(filename.c_str(), &x, &y, &ch, req);

    std::vector<uint8_t*> layer_ptr;
    //uint32_t layers = static_cast<uint32_t>(std::min(log2(x), log2(y)));
    std::cout << x << ' ' << y << ' ' << ch << ' ' << req << '\n';

    std::ofstream ori("MipmapPrototypeOrigin.ppm", std::ios::out);
    ori << "P3\n" << x << ' ' << y << "\n255\n";
    for (int i = 0; i < y; ++i) {
        std::cout << "\rScanline No." << i << ' ' << std::flush;
        for (int j = 0; j < x; ++j) {
            auto* cur = &origin[i * x * req + j * req]; 
            ori << (int)cur[0] << ' ' << (int)cur[1] << ' ' << (int)cur[2] << '\n';
        }
    }
    std::cout << "\nOriginal Done" << std::endl;
    ori.close();


    //uint8_t* buffer = new uint8_t[x * y * req];
    uint8_t* mipmap = new uint8_t[(x / 2) * (y / 2) * req];
    
#if 0
    uint8_t* cur_pixel = buffer;
    for (int i = 0; i < y; ++i) {
        for (int j = 0; j < x - x % 2; j += 2) {
            auto* left = &origin[i * x * req + j * req];
            auto* right = left + req;
            for (int k = 0; k < req; ++k) {
                cur_pixel[k] = left[k] / 2 + right[k] / 2 
                             + (left[k] % 2 + right[k] % 2) / 2;
                // cur_pixel[k] = left[k];
            }
            cur_pixel += req;
        }
    }
    assert(cur_pixel == buffer + (x / 2) * y * req);

    cur_pixel = mipmap;
    for (int i = 0; i < y - y % 2; i += 2) {
        for (int j = 0; j < x / 2; ++j) {
            auto* top = &buffer[i * x / 2 * req + j * req];
            auto* bot = top + x / 2 * req;
            for (int k = 0; k < req; ++k) {
                cur_pixel[k] = top[k] / 2 + bot[k] / 2 
                            + (top[k] % 2 + bot[k] % 2) / 2;
                // cur_pixel[k] = top[k];
            }
            cur_pixel += req;
        }
    }
    assert(cur_pixel = mipmap + (x / 2) * (y / 2) * req);

#else
    uint8_t* cur_pixel = mipmap;
    for (int i = 0; i < y - y % 2; i += 2) {
        for (int j = 0; j < x - x % 2; j += 2) {
            auto* tl = &origin[i * x * req + j * req];
            auto* tr = tl + req;
            auto* bl = tl + x * req;
            auto* br = bl + req;
            for (int k = 0; k < req; ++k) {
                cur_pixel[k] = (tl[k] + tr[k] + bl[k] + br[k]) >> 2;
            }
            cur_pixel += req;
        }
    }
    assert(cur_pixel == mipmap + (x / 2) * (y / 2) * req);
#endif

    int width = x / 2, height = y / 2;
    // std::cout << width << ' ' << height << std::endl;

    std::ofstream ppm("MipmapPrototype.ppm", std::ios::out);
    ppm << "P3\n" << width << ' ' << height << "\n255\n";
    for (int i = 0; i < height; ++i) {
        std::cout << "\rScanline No." << i << ' ' << std::flush;
        for (int j = 0; j < width; ++j) {
            auto* cur = &mipmap[i * width * req + j * req]; 
            ppm << (int)cur[0] << ' ' << (int)cur[1] << ' ' << (int)cur[2] << '\n';
        }
    }
    std::cout << "\nMipmap Done" << std::endl;
    ppm.close();

    delete [] mipmap;

    STBI_FREE(origin);

    return 0;
}


