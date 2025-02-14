#include "Renderer.h"

int main() {

	Renderer renderer(800, 600, "Vulkan Application");

	renderer.init();

	renderer.loop();

	renderer.cleanUp();

	return 0;
}