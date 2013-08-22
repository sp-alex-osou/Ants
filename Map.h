#pragma once

#include "GameComponent.h"
#include "Edge.h"

class Map : public GameComponent
{
public:
	Map(Game& game, const CL_Size& size, const CL_Point& start, const CL_Point& target, float evaporate);

	void draw(float elapsed);

	Edge getEdge(const CL_Point& from, const CL_Point& to) const;
	void setEdge(const CL_Point& from, const CL_Point& to, float pheromone);

	void updateEdges();
	void reset();

	std::vector<CL_Point> getNeighbors(const CL_Point& position) const;
	CL_Point getStart() const;
	bool isTarget(const CL_Point& p) const;
private:
	int getEdgeIndex(const CL_Point& from, const CL_Point& to) const;

	std::vector<Edge> edgesOld;
	std::vector<Edge> edgesNew;

	float evaporate;

	CL_Size size;

	CL_Point start;
	CL_Point target;
};

