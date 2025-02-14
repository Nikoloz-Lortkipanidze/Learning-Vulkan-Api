#include "Renderer.h"
#include "../debug/debug.h"
#include <vector>
#include <set>

void Renderer::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
}

void Renderer::initInstance()
{

	/*std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};*/

	uint32_t extensionCount = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo;
	instanceCreateInfo.enabledExtensionCount = extensionCount;
	instanceCreateInfo.ppEnabledExtensionNames = extensions;
	//instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	//instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();

	debug::check(vkCreateInstance(&instanceCreateInfo, nullptr, &instance), "create instance");
}

bool Renderer::deviceIsSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(device, &properties);

	QueueFamilyLocations locations = findQueueFamilies(device);

	return locations.isComplete();
}

QueueFamilyLocations Renderer::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyLocations locations;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& family : queueFamilies) {
		VkBool32 surfaceSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &surfaceSupport);

		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			locations.graphicsFamily = i;
		}

		if (surfaceSupport) {
			locations.presentFamily = i;
		}

		if (locations.isComplete()) {
			break;
		}

		i++;
	}

	return locations;
}

void Renderer::createLogicalDevice()
{
	QueueFamilyLocations locations = findQueueFamilies(physicalDevice);
	float priority = 1.0f;

	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

	std::set<uint32_t> uniqueQueues = { locations.graphicsFamily.value(), locations.presentFamily.value()};
	std::vector<VkDeviceQueueCreateInfo> queueFamilyInfos;

	for (const auto& queue : uniqueQueues) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queue;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &priority;

		queueFamilyInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo logicalDeviceCreateInfo = {};
	logicalDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	logicalDeviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
	logicalDeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueFamilyInfos.size());
	logicalDeviceCreateInfo.pQueueCreateInfos = queueFamilyInfos.data();

	debug::check(vkCreateDevice(physicalDevice, &logicalDeviceCreateInfo, nullptr, &logicalDevice), "create logical device");
}

void Renderer::createSurface()
{
	debug::check(glfwCreateWindowSurface(instance, window, nullptr, &surface), "create window surface");
}

void Renderer::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	bool deviceIsPicked = false;
	for (const auto& device : devices) {
		if (deviceIsSuitable(device)) {
			physicalDevice = device;
			deviceIsPicked = true;
			break;
		}
	}

	if (!deviceIsPicked) {
		physicalDevice = devices[0];
	}
}

Renderer::Renderer(const int windowWidth, const int windowHeight, const char* windowTitle)
	: width(windowWidth), height(windowHeight), title(windowTitle)
{}

void Renderer::init()
{
	initWindow();
	initInstance();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
}

void Renderer::loop()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void Renderer::cleanUp()
{
	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);
	glfwTerminate();
}
