#pragma once

#include "Include.h"

struct Ant
{
	CL_Point position;
	std::vector<CL_Point> visited;
	float path;

	Ant(const CL_Point& position) : position(position), path(0.0f) {}
};