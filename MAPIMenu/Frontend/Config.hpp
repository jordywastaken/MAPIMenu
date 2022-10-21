
#pragma once

#include <vshlib.hpp>

class Config
{
public:
	Config() = default;

	paf::vec4 color{ 0.9, 0.1, 0.01, 0.6 };
	paf::vec2 position{ 0.0, 0.0 };
	bool enableSounds{ true };
};

extern Config g_Config;