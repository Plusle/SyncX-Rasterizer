#include <core/loader.hpp>

#include <sstream>
#include <iostream>

int main() {
	SyncX::Scene scene;
	SyncX::Loader loader(&scene);

	loader.Execute("./obj/miku.obj");
	loader.Execute("./obj/miku.obj");

	std::cout << "Outside the loader and scene, there are " << scene.GetModels().size() << " models" << std::endl;
	std::cout << scene.GetVertices().size() << " vertices, " << scene.GetTriangles().size() << " faces" << std::endl;

	return 0;
}