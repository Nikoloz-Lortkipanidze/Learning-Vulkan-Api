#pragma once
#include <vulkan/vulkan.h>

namespace debug {
	void success(const char* act);
	void error(const char* act);

	bool check(bool res, const char* act);
	bool check(VkResult res, const char* act);

}
