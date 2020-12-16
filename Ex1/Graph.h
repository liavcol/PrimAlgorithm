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
	//Constructors
	Graph();
	Graph(const Graph& og);
	
	//Operators
	const Graph& operator=(const Graph& other);

	//Getters
	const std::vector<Edge*>& Edges() const;
	const std::unordered_map<char, Vertex*> Vertices() const;

	//Methods to Manage the Graph
	bool AddVertex(char name); //Adds only if the vertex does't exist.
	bool AddEdge(char v1, char v2, float weight); //Adds only if both vertices exist (notice that multiple edges between vertices are allowed).
	bool RemoveVertex(char name);
	bool RemoveEdge(char v1, char v2); //Works at O(|E|) to find the edge.
	Edge* FindEdge(char v1, char v2) const;

	//Task 1:
	//Creates a new graph which is an MST using Prim's algorithm.
	//Works at O(|E|log(|V|).
	Graph MST_Prim(); 
	
	//Taks 2:
	//Given an already calculated MST of this graph and info for a new edge, it adds the edge and calculate the new MST efficiently.
	//Works at O(|E| + |V|)
	Graph& AddEdgeAndUpdateMST(Graph& mst, char v1, char v2, float weight); 
	
	//Friend Functions
	friend std::ostream& operator<<(std::ostream& out, const Graph& g);

	~Graph();
};

