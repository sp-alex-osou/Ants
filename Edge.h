#pragma once

struct Edge
{
	float pheromone;
	float length;

	Edge(float pheromone = 0.0f, float length = 1.0f) : pheromone(pheromone), length(length) {}
};