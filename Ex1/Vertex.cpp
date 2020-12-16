#include "Vertex.h"

Vertex::Vertex(char name) : _name(name) { }

char Vertex::GetName() const { return _name; }

void Vertex::AddAdjacent(Edge* e) { _adj.push_back(e); }

void Vertex::RemoveAdjacent(Edge* e)
{
	for (std::list<Edge*>::iterator it = _adj.begin(); it != _adj.end(); it++)
	{
		if (*it == e)
		{
			_adj.erase(it);
			return;
		}
	}
}

const std::list<Edge*>& Vertex::Adj() const { return _adj; }