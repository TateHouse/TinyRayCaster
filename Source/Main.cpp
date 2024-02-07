#include "Application.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
	try {
		TinyRayCaster::Application application {};
		application.run();
	}
	catch (const std::exception& exception) {
		std::cerr << exception.what() << '\n';
		return -1;
	}
	
	return 0;
}