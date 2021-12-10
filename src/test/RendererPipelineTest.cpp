#include <core/io_interface.hpp>
#include <core/scene.hpp>
#include <core/loader.hpp>
#include <core/pipeline.hpp>




int main(int argc, char** argv) {
    //uint32_t width = std::atoi(argv[1]), height = std::atoi(argv[2]);
    uint32_t width = 400, height = 300;
    SyncX::ImagePrinter printer("pipeline_test.ppm", width, height);
    SyncX::Scene sc;
    SyncX::Loader loader(&sc);
    loader.Execute("./obj/miku.obj");
    printer.Init(&sc);
    printer.Display();
    





	return 0;
}