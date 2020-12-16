#pragma once

#include "Edge.h"
#include <list>

class Edge;

class Vertex
{
private:
	char _name;
	std::list<Edge*> _adj;

public:
	Vertex(char name);

	char GetName() const;

	void AddAdjacent(Edge* e);
	void RemoveAdjacent(Edge* e);
	const std::list<Edge*>& Adj() const;
};

