#include "Graph.h"
#include <queue>

Graph::Graph() { }

Graph::Graph(const Graph& other)
{
	*this = other;
}

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

void Graph::AddVertex(char name)
{
	_V[name] = new Vertex(name);
}

void Graph::AddEdge(char v1, char v2, int weight)
{
	if (_V.find(v1) == _V.end() || _V.find(v2) == _V.end())
		return;
	Edge* e = new Edge(_V[v1], _V[v2], weight);
	_V[v1]->AddAdjacent(e);
	_V[v2]->AddAdjacent(e);
	_E.push_back(e);
}

void Graph::RemoveEdge(char v1, char v2)
{
	for (unsigned int i = 0; i < _E.size(); i++)
	{
		if (_E[i]->CheckSides(v1, v2))
		{
			_V[v1]->RemoveAdjacent(_E[i]);
			_V[v2]->RemoveAdjacent(_E[i]);
			delete _E[i];
			_E.erase(_E.begin() + i);
			return;
		}
	}
}

Graph Graph::MST_Prim()
{
	struct PrimData
	{
		char name;
		float key;
		Vertex* pi;
		bool inQueue;
		struct CompareKey
		{
			bool operator()(const PrimData* v1, const PrimData* v2)
			{
				return v1->key > v2->key;
			}
		};
	};

	std::unordered_map<char, PrimData*> primDataMap;
	for (std::pair<char, Vertex*> v : _V)
		primDataMap[v.first] = new PrimData { v.first, INFINITY, nullptr, true };

	std::priority_queue<PrimData*, std::vector<PrimData*>, PrimData::CompareKey> q;
	(*(primDataMap.begin())).second->key = 0;
	q.push((*(primDataMap.begin())).second);

	while (!q.empty())
	{
		PrimData* u = q.top();
		q.pop();
		u->inQueue = false;
		for (Edge* e : _V[u->name]->Adj())
		{
			PrimData* v = primDataMap[e->GetOtherSide(u->name)];
			if (v->inQueue && e->GetWeight() < v->key)
			{
				v->pi = _V[u->name];
				v->key = (float)e->GetWeight();
				q.push(v);
			}
		}
	}

	Graph mst;
	for (std::pair<char, Vertex*> v : _V)
		mst.AddVertex(v.first);
	
	for (std::pair<char, PrimData*> pd : primDataMap)
		if(pd.second->pi)
			mst.AddEdge(pd.second->pi->GetName(), pd.second->name, (int)pd.second->key);

	for (std::pair<char, PrimData*> pd : primDataMap)
		delete pd.second;

	return mst;
}

Graph& Graph::AddEdgeAndUpdateMST(Graph& mst, char v1, char v2, int weight)
{
	struct CycleFinder
	{
		struct CycleData
		{
			char name;
			char color;
			Vertex* pi;
			int edgeWeight;
		};
		
		static Vertex* FindCycleEdge(Vertex* v, std::unordered_map<char, CycleFinder::CycleData*>& cycleDataMap)
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
					return FindCycleEdge(v == e->GetV1() ? e->GetV2() : e->GetV1(), cycleDataMap);
				}
			}
			cycleDataMap[v->GetName()]->color = 'B';
			return nullptr;
		}
	};

	AddEdge(v1, v2, weight);
	mst.AddEdge(v1, v2, weight);

	std::unordered_map<char, CycleFinder::CycleData*> cycleDataMap;
	for (std::pair<char, Vertex*> v : mst._V)
		cycleDataMap[v.second->GetName()] = new CycleFinder::CycleData {v.first, 'W', nullptr, 0};

	for (std::pair<char, Vertex*> v : mst._V)
	{
		if (cycleDataMap[v.second->GetName()]->color == 'W')
		{
			Vertex* cycleVertex = CycleFinder::FindCycleEdge(v.second, cycleDataMap);
			if (cycleVertex != nullptr)
			{
				Vertex* p = cycleVertex;
				do
				{
					if (cycleDataMap[p->GetName()]->edgeWeight > weight)
					{
						mst.RemoveEdge(p->GetName(), cycleDataMap[p->GetName()]->pi->GetName());
						for (std::pair<char, CycleFinder::CycleData*> pc : cycleDataMap)
							delete pc.second;
						return mst;
					}
					p = cycleDataMap[p->GetName()]->pi;
				} while (p != cycleVertex);
			}
		}
	}

	mst.RemoveEdge(v1, v2);
	for (std::pair<char, CycleFinder::CycleData*> pc : cycleDataMap)
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