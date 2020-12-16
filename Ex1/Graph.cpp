#include "Graph.h"
#include <queue>

Graph::Graph() { }

Graph::Graph(const Graph& other) { *this = other; }

const Graph& Graph::operator=(const Graph& other)
{
	if (&other != this)
	{
		for (std::pair<char, Vertex*> v : _V)
			delete v.second;
		for (Edge* e : _E)
			delete e;

		for (std::pair<char, Vertex*> v : other._V)
			AddVertex(v.first);

		for (Edge* e : other._E)
			AddEdge(e->GetV1()->GetName(), e->GetV2()->GetName(), e->GetWeight());
	}

	return *this;
}

const std::vector<Edge*>& Graph::Edges() const { return _E; }

const std::unordered_map<char, Vertex*> Graph::Vertices() const { return _V; }

bool Graph::AddVertex(char name) 
{
	if (_V.find(name) != _V.end())
		return false;
	_V[name] = new Vertex(name);
	return true;
}

bool Graph::AddEdge(char v1, char v2, float weight)
{
	if (_V.find(v1) == _V.end() || _V.find(v2) == _V.end())
		return false;
	Edge* e = new Edge(_V[v1], _V[v2], weight);
	_V[v1]->AddAdjacent(e);
	_V[v2]->AddAdjacent(e);
	_E.push_back(e);
	return true;
}

bool Graph::RemoveVertex(char name)
{
	if (_V.find(name) == _V.end())
		return false;
	_V.erase(name);
	return true;
}

bool Graph::RemoveEdge(char v1, char v2)
{
	for (unsigned int i = 0; i < _E.size(); i++)
	{
		if (_E[i]->CheckSides(v1, v2))
		{
			_V[v1]->RemoveAdjacent(_E[i]);
			_V[v2]->RemoveAdjacent(_E[i]);
			delete _E[i];
			_E.erase(_E.begin() + i);
			return true;
		}
	}
	return false;
}

Edge* Graph::FindEdge(char v1, char v2) const
{
	for (unsigned int i = 0; i < _E.size(); i++)
		if (_E[i]->CheckSides(v1, v2))
			return _E[i];
	return nullptr;
}

Graph Graph::MST_Prim()
{
	if (_V.empty() || _E.empty())
		return Graph();

	struct PrimData //Local struct to organize data needed by Prim's algorithm.
	{
		char name;
		float key;
		Vertex* pi;
		bool inQueue; //To check if the vertex is in queue at O(1).
		struct CompareKey //Defines a comparator object for use by a priority queue.
		{
			bool operator()(const PrimData* v1, const PrimData* v2)
			{
				return v1->key > v2->key;
			}
		};
	};

	//For each vertex, we'll create a new PrimData object to hold it's info for the algorithm and we'll organize them in a hash map for fast lookup by name.
	//Note that we initialize every vertex data with a key of infinity and pi of null as needed by Prim. Also, all the vertices are in queue by default.
	std::unordered_map<char, PrimData*> primDataMap; 
	for (std::pair<char, Vertex*> v : _V)
		primDataMap[v.first] = new PrimData { v.first, INFINITY, nullptr, true };

	//Building the priority queue.
	//Since prim lets us use any vertex as the starting vertex we start with the first one.
	//Also, since updating the key won't re-organize the queue, we'll push vertices to the queue only when we update their key field. 
	std::priority_queue<PrimData*, std::vector<PrimData*>, PrimData::CompareKey> q;
	(*(primDataMap.begin())).second->key = 0; //Set key to 0.
	q.push((*primDataMap.begin()).second);
	
	while (!q.empty())
	{
		PrimData* u = q.top(); //Extract-Min
		q.pop();
		u->inQueue = false;
		for (Edge* e : _V[u->name]->Adj())
		{
			PrimData* v = primDataMap[e->GetOtherSide(u->name)];
			if (v->inQueue && e->GetWeight() < v->key)
			{
				v->pi = _V[u->name];
				v->key = e->GetWeight();
				q.push(v);
			}
		}
	}
	//End of Prim's algorithm

	//Building the mst graph object at O(mst.|V| + mst.|E|) <= O(|E|log(|V|)
	Graph mst;
	for (std::pair<char, Vertex*> v : _V)
		mst.AddVertex(v.first);
	
	for (std::pair<char, PrimData*> pd : primDataMap)
		if(pd.second->pi)
			mst.AddEdge(pd.second->pi->GetName(), pd.second->name, pd.second->key);

	//Deleting heap allocated memory used by method.
	for (std::pair<char, PrimData*> pd : primDataMap)
		delete pd.second;

	return mst;
}

//The algorithm uses a modification of the DFS algorithm to locate a back edge (indicates a cycle) that will be created in the mst by the addition of an |V|'th edge.
//After finding the cycle this way we remove the heaviest edge. The algorithm runs at a runtime of DFS (O(|E| + |V|)), 
//but since we're running it on the mst |E| = |V| and we get a runtime of O(|V|).
Graph& Graph::AddEdgeAndUpdateMST(Graph& mst, char v1, char v2, float weight)
{
	struct CycleData //Local struct to organize data needed by DFS to find a back edge. 
	{
		char name;
		char color; //'W' - White / 'G' - Gray / 'B' - Black
		Vertex* pi;
		float edgeWeight; //The weight of the edge connecting pi and the vertex.

		//This static method is basically the DFS-Visit function of the DFS algorithm, modified to locate and return a vertex that is connected to a back edge.
		static Vertex* FindCycleEdge(Vertex* v, std::unordered_map<char, CycleData*>& cycleDataMap)
		{
			cycleDataMap[v->GetName()]->color = 'G';
			for (Edge* e : v->Adj())
			{
				CycleData* cd = cycleDataMap[e->GetOtherSide(v->GetName())];
				if (cd->color == 'G' && cd->name != cycleDataMap[v->GetName()]->pi->GetName())
				{
					cd->pi = v;
					cd->edgeWeight = e->GetWeight();
					return v == e->GetV1() ? e->GetV2() : e->GetV1();
				}
				if (cd->color == 'W')
				{
					cd->pi = v;
					cd->edgeWeight = e->GetWeight();
					Vertex* cv = FindCycleEdge(v == e->GetV1() ? e->GetV2() : e->GetV1(), cycleDataMap);
					if (cv)
						return cv;
				}
			}
			cycleDataMap[v->GetName()]->color = 'B';
			return nullptr;
		}
	};
	
	if(!(AddEdge(v1, v2, weight) && mst.AddEdge(v1, v2, weight)))
		return mst;

	//For each vertex in the mst, we'll create a new PrimData object to hold it's info for the algorithm and we'll organize them in a hash map for fast lookup by name.
	//Note that we initialize every vertex data with a color of white and pi of null as needed by DFS.
	std::unordered_map<char, CycleData*> cycleDataMap;
	for (std::pair<char, Vertex*> v : mst._V)
		cycleDataMap[v.second->GetName()] = new CycleData{ v.first, 'W', nullptr, 0 };
	
	//A cycle is guaranteed to returned.
	Vertex* cycleVertex = CycleData::FindCycleEdge((*mst._V.begin()).second, cycleDataMap);
	if (cycleVertex)
	{
		Vertex* p = cycleVertex;
		do //Iterating the cycle vertices using the pi field of the vertex...
		{
			if (cycleDataMap[p->GetName()]->edgeWeight > weight) //...until we find an edge heavier than the the one we added and remove it.
			{
				mst.RemoveEdge(p->GetName(), cycleDataMap[p->GetName()]->pi->GetName());
				for (std::pair<char, CycleData*> pc : cycleDataMap)
					delete pc.second;
				return mst;
			}
			p = cycleDataMap[p->GetName()]->pi;
		} while (p != cycleVertex);
	}

	mst.RemoveEdge(v1, v2);
	for (std::pair<char, CycleData*> pc : cycleDataMap)
		delete pc.second;
	
	return mst;
}

std::ostream& operator<<(std::ostream& out, const Graph& g)
{
	out << "Vertices [" << g._V.size() << "]:" << std::endl;
	for (std::pair<char, Vertex*> v : g._V)
		out << v.first << std::endl;
	out << "Edges [" << g._E.size() << "]:" << std::endl;
	for (Edge* e : g._E)
		out << e->GetV1()->GetName() << "<--->" << e->GetV2()->GetName() << " w: " << e->GetWeight() << std::endl;
	
	return out;
}

Graph::~Graph()
{
	for (Edge* e : _E)
		delete e;
	for (std::pair<char, Vertex*> v : _V)
		delete v.second;
}