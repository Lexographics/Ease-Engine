#include <iostream>

#include "core/application.hpp"
#include "visual/visual.hpp"

int main(int argc, char const *argv[]) {
	{
		Application app(argc, argv);
		try {
			app.Init();
		} catch (const std::exception &e) {
			std::cerr << e.what() << '\n';
		}
	}

	Visual::DestroyState();
	return 0;
}
