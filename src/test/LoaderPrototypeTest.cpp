#include <core/loader.hpp>

#include <sstream>
#include <iostream>

int main() {
	SyncX::Scene scene;
	SyncX::Loader loader(&scene);

	loader.Execute("./obj/Marry.obj");
	loader.Execute("./obj/floor.obj");

	std::cout << "In the end, there are " << scene.GetModels().size() << " models" << std::endl;
	std::cout << scene.GetVertices().size() << " vertices, " << scene.GetTriangles().size() << " faces" << std::endl;

	return 0;
}