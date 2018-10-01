#include <iostream>
#include <unordered_set>
#include <fstream>
#include <iterator>

using namespace std;
// Using a graph structure with an array of unordered_sets
// An edge between two chemicals represents they react with each other
// since unordered_sets are stored as hash tables in C++
// Adding or searching for an element both take O(1) time in a set
struct Graph{
	int V;
	unordered_set<int>* edges;

	Graph(int N) : V(N)
	{
		edges = new unordered_set<int>[N];
	}
};
// Assume directed graph: add second vertex into edge of first vertex
void insert_edge(Graph* graph, int u, int v){
	graph->edges[u].insert(v);
}

// This function takes on average O(1) time since sets are stored as hash tables
// But in worst case it will take O(|V| + |E|) time, when:
	// there is an edge from each vertex to every other vertex
bool check_edge(const Graph* graph, int u, int v){
	auto itr = graph->edges[u].find(v);

	itr != graph->edges[u].end() ? true : false;
}
