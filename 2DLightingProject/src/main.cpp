#include "Handler.hpp"

int main() {

	Handler program;

	while (program.running()) {

		program.handleEvents();
		program.render();

	}

	return 0;
}