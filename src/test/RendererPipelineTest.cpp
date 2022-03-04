#include <core/io_interface.hpp>
#include <core/scene.hpp>
#include <core/loader.hpp>
#include <core/pipeline.hpp>

using namespace SyncX;

int main(int argc, char** argv) {
    //uint32_t width = std::atoi(argv[1]), height = std::atoi(argv[2]);
    uint32_t width = 1600, height = 900;
    ImagePrinter printer("pipeline_test.ppm", width, height);
    Scene sc;
    Loader loader(&sc);
    loader.Execute("./obj/Mary.obj");
    //loader.Execute("./obj/cube2.obj");

    printer.Init(&sc);
    printer.Display();
    





	return 0;
}