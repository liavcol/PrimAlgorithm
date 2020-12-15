#include "Graph.h"
#include <iostream>
#include <ctime>

int main()
{
	Graph g;
	char n = 'a';
	for (int i = 0; i < 20; i++)
	{
		g.AddVertex(n);
		n++;
	}

	srand(time(0));
	for (int i = 0; i < 50; i++)
	{
		char v1;
		char v2;
		do
		{
			v1 = 'a' + rand() % 20;
			v2 = 'a' + rand() % 20;
		} while (v1 == v2);
		g.AddEdge(v1, v2, rand() % 100);
	}

	std::cout << "Graph:" << std::endl;
	std::cout << g;

	Graph mst = g.MST_Prim();

	std::cout << std::endl << "Generated MST:" << std::endl;

	std::cout << mst;

	
	mst = g.AddEdgeAndUpdateMST(mst, 'c', 'd', 4);

	std::cout << std::endl << "New Generated MST: " << std::endl;

	std::cout << mst;
	
	std::cin.get();
}