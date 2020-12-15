#include "Edge.h"

Edge::Edge(Vertex* v1, Vertex* v2, int weight) : _v1(v1), _v2(v2), _weight(weight) { }

Vertex* Edge::GetV1() const { return _v1; }

Vertex* Edge::GetV2() const { return _v2; }

char Edge::GetThisSide(char otherSide) const
{
	return otherSide == _v1->GetName() ? _v1->GetName() : _v2->GetName();
}

char Edge::GetOtherSide(char thisSide) const
{
	return thisSide == _v1->GetName() ? _v2->GetName() : _v1->GetName();
}

bool Edge::CheckSides(char sideA, char sideB) const
{
	return _v1->GetName() == sideA && _v2->GetName() == sideB || _v1->GetName() == sideB && _v2->GetName() == sideA;
}

int Edge::GetWeight() const { return _weight; }