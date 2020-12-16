#pragma once

#include "Vertex.h"

class Vertex;

class Edge
{
private:
	Vertex* _v1;
	Vertex* _v2;
	float _weight;

public:
	Edge(Vertex* v1, Vertex* v2, float weight);
	
	Vertex* GetV1() const;
	Vertex* GetV2() const;
	float GetWeight() const;

	char GetOtherSide(char thisSide) const; //Given a vertex name, it gets the other vertex name.
	bool CheckSides(char sideA, char sideB) const; //Checks to see if edge connects 2 given vertices.
};

