#pragma once
#include "Graph.h"
#include <map>
#include <set>
#include <queue>
typedef Node <string, int> TNode;
typedef Rib<TNode, int> TRib;
typedef Graph <TNode, TRib> TGraph;

template <class NodeT, class RibT>
class Classification {
private:
	Graph<NodeT, RibT>* graph;
	NodeT* Node;
	stringstream res;

	struct top
	{
		int color;
		int d;
		NodeT* p;
		int f;
	};
	int time;
	map<NodeT*, top> vtop;
	set<NodeT*> visited;
	queue<RibT*> path;
	string AnalyzeRib(NodeT* u, NodeT* node)
	{
		string RibType = "";
		if (vtop[u].d < vtop[node].d && vtop[u].f > vtop[node].f&& vtop[node].p == u)
			RibType = "Древесное";
		if (vtop[u].d > vtop[node].d&& vtop[u].f < vtop[node].f)
			RibType = "Обратное";
		if (vtop[u].d < vtop[node].d && vtop[u].f > vtop[node].f&& vtop[node].p != u)
			RibType = "Прямое";
		if (vtop[u].d > vtop[node].d&& vtop[u].f > vtop[node].f)
			RibType = "Поперечное";
		return RibType;
	}

	void DFS_path_output(NodeT* u)
	{
		TGraph::OutputRibIterator* itOut = new TGraph::OutputRibIterator(*graph, *u);
		RibT* curRib = NULL;
		itOut->begin();
		visited.insert(u);
		while (!(itOut->onEnd()))
		{
			curRib = *(*itOut);
			NodeT* node = NULL;
			if (curRib->getNode_1() == u)
				node = curRib->getNode_2();
			else
				node = curRib->getNode_1();
			res << u->getName() << "->" << node->getName() << " - " << AnalyzeRib(u, node) << endl;
			if (visited.find(node) == visited.end())
				DFS_path_output(node);
			(*itOut)++;
		}
	}
	void DFS_path_all()
	{
		while (!path.empty())
		{
			RibT* curRib = path.front();
			path.pop();
			NodeT* u = curRib->getNode_1();
			NodeT* node = curRib->getNode_2();
			res << u->getName() << "->" << node->getName() << " - " << AnalyzeRib(u, node) << endl;
		}
	}
	void DFS_visit_all()
	{
		vtop.clear();
		time = 0;

		TGraph::NodeIterator* it = new TGraph::NodeIterator(*graph);
		it->begin();
		while (!(it->onEnd()))
		{
			vtop[*(*it)].color = 0;
			vtop[*(*it)].p = NULL;
			++(*it);
		}

		DFS_visit(Node);

		it->begin();
		while (!(it->onEnd()))
		{
			if (vtop[*(*it)].color == 0)
				DFS_visit(*(*it));
			++(*it);
		}
	}
	void DFS_visit(NodeT* u)
	{
		vtop[u].color = 1;
		vtop[u].d = ++time;

		TGraph::OutputRibIterator* itout = new TGraph::OutputRibIterator(*graph, *u);

		itout->begin();
		RibT* cRib = NULL;

		while (!(itout->onEnd()))
		{
			cRib = *(*itout);
			path.push(cRib);
			NodeT* node = NULL;
			if (cRib->getNode_1() == u)
				node = cRib->getNode_2();
			else
				node = cRib->getNode_1();

			if (vtop[node].color == 0)
			{
				vtop[node].p = u;
				DFS_visit(node);
			}
			++(*itout);
		}
		vtop[u].color = 2;
		vtop[u].f = time++;
	}
public:
	Classification(Graph<NodeT, RibT>* g, NodeT* node) :
		graph(g), Node(node)
	{
		restart();
	}
	Classification(const Classification<NodeT, RibT>& t1) :
		graph(t1.g), Node(t1.node) {}

	void set(Graph<NodeT, RibT>* g, NodeT* node) {
		graph = g;
		Node = node;
		restart();
	}
	void restart() {
		res.str("");
		DFS_visit_all();
		visited.clear();
		DFS_path_all();
	}
	string result()
	{
		return res.str();
	}
};

