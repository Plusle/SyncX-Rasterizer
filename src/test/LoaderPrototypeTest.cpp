#include <core/loader.hpp>

#include <sstream>
#include <iostream>

int main() {
	SyncX::Scene scene;
	SyncX::Loader loader(&scene);

	loader.Execute("./obj/miku.obj");

	return 0;
}