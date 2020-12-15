#pragma once

#include "Edge.h"
#include "Vertex.h"
#include <iostream>
#include <vector>
#include <unordered_map>

class Graph
{
private:
	std::unordered_map<char, Vertex*> _V;
	std::vector<Edge*> _E;

public:
	Graph();
	Graph(const Graph& og);
	void AddVertex(char name);
	void AddEdge(char v1, char v2, int weight);
	void RemoveEdge(char v1, char v2);
	const Graph& operator=(const Graph& other);
	Graph MST_Prim(); 
	Graph& AddEdgeAndUpdateMST(Graph& mst, char v1, char v2, int weight);

	friend std::ostream& operator<<(std::ostream& out, const Graph& g);

	~Graph();
};

