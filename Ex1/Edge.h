#pragma once

#include "Vertex.h"

class Vertex;

class Edge
{
private:
	Vertex* _v1;
	Vertex* _v2;
	int _weight;

public:
	Edge(Vertex* v1, Vertex* v2, int weight);
	Vertex* GetV1() const;
	Vertex* GetV2() const;
	char GetThisSide(char otherSide) const;
	char GetOtherSide(char thisSide) const;
	bool CheckSides(char sideA, char sideB) const;
	int GetWeight() const;
};

