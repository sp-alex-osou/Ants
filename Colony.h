#pragma once

#include "GameComponent.h"
#include "Ant.h"
#include "Map.h"

class Colony : public GameComponent
{
public:
	Colony(Game& game, int size, int considerDistance, int considerPheromone, Map& map);
private:
	Map& map;
	int size;
	int considerDistance;
	int considerPheromone;

	void update(float elapsed);
	void simulate();

	struct Option
	{
		CL_Point position;
		Edge edge;
		float probability;

		Option(const CL_Point& position, const Edge& edge, float probability)
			: position(position), edge(edge), probability(probability) {}
	};
};