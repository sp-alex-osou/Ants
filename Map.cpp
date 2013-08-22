#include "Map.h"

Map::Map(Game& game, const CL_Size& size, const CL_Point& start, const CL_Point& target, float evaporate) 
	: GameComponent(game), size(size), start(start), target(target), evaporate(evaporate)
{
	edgesOld = std::vector<Edge>(size.width * size.height * 2);
	edgesNew = edgesOld;
}


Edge Map::getEdge(const CL_Point& from, const CL_Point& to) const
{
	return edgesOld[getEdgeIndex(from, to)];
}


void Map::setEdge(const CL_Point& from, const CL_Point& to, float pheromone)
{
	edgesNew[getEdgeIndex(from, to)].pheromone += pheromone;
}


void Map::updateEdges()
{
	for (int i = 0; i < (int)edgesOld.size(); ++i)
	{
		edgesOld[i].pheromone = (1.0f - evaporate) * edgesOld[i].pheromone + edgesNew[i].pheromone;
		edgesNew[i].pheromone = 0.0f;
	}
}


void Map::reset()
{
	for (std::vector<Edge>::iterator it = edgesOld.begin(); it != edgesOld.end(); ++it)
		it->pheromone = 0.0f;
}


int Map::getEdgeIndex(const CL_Point& from, const CL_Point& to) const
{
	CL_Point p = (from.x < to.x || from.y < to.y) ? from : to;

	return (p.y * size.width + p.x) * 2 + (from.y == to.y);
}


CL_Point Map::getStart() const
{
	return start;
}


bool Map::isTarget(const CL_Point& p) const
{
	return p == target;
}


std::vector<CL_Point> Map::getNeighbors(const CL_Point& position) const
{
	std::vector<CL_Point> neighbors;

	if (position.x > 0)
		neighbors.push_back(CL_Point(position.x - 1, position.y));

	if (position.x < size.width - 1)
		neighbors.push_back(CL_Point(position.x + 1, position.y));

	if (position.y > 0)
		neighbors.push_back(CL_Point(position.x, position.y - 1));

	if (position.y < size.height - 1)
		neighbors.push_back(CL_Point(position.x, position.y + 1));

	return neighbors;
}


void Map::draw(float elapsed)
{
	CL_Vec2<float> fieldSize;
	CL_Pointf p;

	CL_Rect viewport = window.get_viewport();
	
	fieldSize.x = (float)viewport.get_width() / size.width;
	fieldSize.y = (float)viewport.get_height() / size.height;

	for (int row = 0; row < size.height; row++)
		for (int col = 0; col < size.width; col++)
		{
			p = CL_Pointf(col * fieldSize.x, row * fieldSize.y) + fieldSize / 2;	

			if (row < size.height - 1)
			{
				CL_Colorf color(getEdge(CL_Point(col, row), CL_Point(col, row + 1)).pheromone / 10, 0, 0);
				CL_Draw::line(window.get_gc(), p, p + CL_Vec2<float>(0, fieldSize.y), color);
			}

			if (col < size.width - 1)
			{
				CL_Colorf color(getEdge(CL_Point(col, row), CL_Point(col + 1, row)).pheromone / 10, 0, 0);
				CL_Draw::line(window.get_gc(), p, p + CL_Vec2<float>(fieldSize.x, 0), color);
			}
		}

	p = CL_Pointf(start.x * fieldSize.x, start.y * fieldSize.y);
	CL_Draw::fill(window.get_gc(), p, p + fieldSize, CL_Colorf::red);

	p = CL_Pointf(target.x * fieldSize.x, target.y * fieldSize.y);
	CL_Draw::fill(window.get_gc(), p, p + fieldSize, CL_Colorf::green);
}