#define MIPMAP_DEBUG_INFO
#include <core/texture.hpp>

#include <iostream>

int main(int argc, char** argv) {
    SyncX::Texture t("texture.png");
    for (const auto& compacity : t.layer_compacity) {
        std::cout << compacity << ' ';
    }
    return 0;
}