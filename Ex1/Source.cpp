#include "Graph.h"
#include <iostream>
#include <ctime>

//Task 3
int main()
{
	//Creating a graph with 20 vertices and 50 random edges.
	Graph g;
	//20 vertices named a..t.
	char n = 'a';
	for (int i = 0; i < 20; i++)
	{
		g.AddVertex(n);
		n++;
	}

	//19 edges with random weights that ensure a connected graph for the mst showcase.
	char v1 = 'a', v2 = 'b';
	srand(time(0));
	for (int i = 0; i < 19; i++)
	{
		g.AddEdge(v1, v2, (float)(rand() % 100));
		v1++;
		v2++;
	}
	//Another 31 random edges.
	for (int i = 0; i < 31; i++)
	{
		do
		{
			v1 = 'a' + rand() % 20;
			v2 = 'a' + rand() % 20;
		} while (v1 == v2 && !g.FindEdge(v1, v2));
		g.AddEdge(v1, v2, (float)(rand() % 100));
	}

	std::cout << "Graph:" << std::endl << g << std::endl;

	//Running the MST_Prim function (Task 1).
	Graph mst = g.MST_Prim();
	std::cout << std::endl << "Generated MST:" << std::endl << mst << std::endl;

	//Find the highest weight in the mst to generate an edge that won't change that mst and an edge that will (negative of the highest weight).
	float maxWeight = 0;
	for (Edge* e : mst.Edges())
		if (e->GetWeight() > maxWeight)
			maxWeight = e->GetWeight();
	do
	{
		v1 = 'a' + rand() % 20;
		v2 = 'a' + rand() % 20;
	} while (v1 == v2 && !g.FindEdge(v1, v2));
	std::cout << "Edge: " << v1 << " <---> " << v2 << " w: " << maxWeight << " WON'T change the mst: " << std::endl;
	mst = g.AddEdgeAndUpdateMST(mst, v1, v2, maxWeight);
	std::cout <<"MST after adding edge:" << std::endl << mst << std::endl;

	std::cout << "Edge " << v1 << " <---> " << v2 << " w: " << -maxWeight << " WILL change the mst: " << std::endl;
	mst = g.AddEdgeAndUpdateMST(mst, v1, v2, -maxWeight);
	std::cout << "MST after adding edge:" << std::endl << mst << std::endl;

	std::cin.get();
}