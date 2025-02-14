#include "debug.h"
#include <iostream>

void debug::success(const char* act)
{
	std::cout << "SUCCESS -> " << act << '\n';
}

void debug::error(const char* act)
{
	std::cout << "ERROR -> " << act << '\n';
}

bool debug::check(bool res, const char* act)
{
	if (!res) {
		error(act);
		return false;
	}

	success(act);
	return true;
}

bool debug::check(VkResult res, const char* act)
{
	if (res != VK_SUCCESS) {
		error(act);
		return false;
	}

	success(act);
	return true;
}
