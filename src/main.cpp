#include <iostream>

#include "core/application.hpp"
#include "visual/visual.hpp"

int main(int argc, char const *argv[]) {

	try {
		Application app(argc, argv);
		app.Start();
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}

	Visual::DestroyState();
	return 0;
}
