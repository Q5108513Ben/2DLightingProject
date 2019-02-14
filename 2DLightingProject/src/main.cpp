#include "Handler.hpp"

int main() {

	Handler program;

	while (program.Running()) {

		program.HandleEvents();
		program.Render();

	}

	return 0;
}