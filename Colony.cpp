#include "Colony.h"

#include <algorithm>
#include <ctime>

Colony::Colony(Game& game, int size, int considerDistance, int considerPheromone, Map& map) 
	: GameComponent(game), size(size), map(map), considerDistance(considerDistance), considerPheromone(considerPheromone)
{
	std::srand(0);
}


void Colony::update(float elapsed)
{
	simulate();
}


void Colony::simulate()
{
	CL_Point start = map.getStart();

	for (int i = 0; i < size; ++i)
	{
		Ant ant(start);

		while (!map.isTarget(ant.position))
		{
			std::vector<CL_Point> neighbors = map.getNeighbors(ant.position);
			std::vector<Option> options;
			float totalProbability = 0;

			for (std::vector<CL_Point>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
			{
				if (std::find(ant.visited.begin(), ant.visited.end(), *it) == ant.visited.end())
				{
					Edge edge = map.getEdge(ant.position, *it);
					float probability = std::pow(edge.pheromone, considerPheromone) * std::pow(1.0f / edge.length, considerDistance);
					options.push_back(Option(*it, edge, probability));
					totalProbability += probability;
				}
			}

			if (options.empty())
				break;

			int current = 0;

			if (totalProbability > 0.0f)
			{
				float rand = (std::rand() / (float)RAND_MAX) * totalProbability;

				while ((rand -= options[current].probability) > 0.0f && current < (int)options.size() - 1)
					++current;
			}
			else
				current = std::rand() % options.size();

			ant.visited.push_back(ant.position);
			ant.path += options[current].edge.length;
			ant.position = options[current].position;
		}

		if (!map.isTarget(ant.position))
			continue;

		ant.visited.push_back(ant.position);

		float optimalPath = (float)(std::abs(start.x - ant.position.x) + std::abs(start.y - ant.position.y));
		float pheromone = optimalPath / ant.path;

		for (std::vector<CL_Point>::iterator it = ant.visited.begin(); it != ant.visited.end() - 1; ++it)
			map.setEdge(*it, *(it+1), pheromone);
	}

	map.updateEdges();
}