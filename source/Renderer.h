#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

struct QueueFamilyLocations {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class Renderer
{
private:
	GLFWwindow* window = NULL;
	const int width, height;
	const char* title;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice;
	VkSurfaceKHR surface;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	void initWindow();
	void initInstance();
	void pickPhysicalDevice();
	bool deviceIsSuitable(VkPhysicalDevice device);
	QueueFamilyLocations findQueueFamilies(VkPhysicalDevice device);
	void createLogicalDevice();
	void createSurface();
public:
	Renderer(const int windowWidth, const int windowHeight, const char* windowTitle);
	void init();
	void loop();
	void cleanUp();
};

