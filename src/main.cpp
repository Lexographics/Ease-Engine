#include <iostream>

#include "core/application.hpp"

int main(int argc, char const *argv[]) {
	Application app(argc, argv);

	try {
		app.Start();
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}

	return 0;
}
