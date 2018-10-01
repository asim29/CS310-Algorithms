#include <iostream>
#include <unordered_set>
#include <fstream>
#include <iterator>
#include <sstream>

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

// this function takes O(|V| + |E|) time since the time required
	// to add an element in a set is O(1)
Graph* parse_file(string filename){
	string line;
	ifstream f(filename);
	if(f.is_open()){
		getline(f, line);
		// remove "n " and convert the rest to integer
		int n = stoi(line.substr(2));

		Graph* graph = new Graph(n);
		while(getline(f, line)){
			// find position of :
			int end = line.find(":");

			// find left side number
			int chem1 = stoi(line.substr(0,end).substr(1));
			// find right side numbers
			string reacts = line.substr(end+1);

			vector<string> tokens;
			string token;
			istringstream tokenStream(reacts);
			// find vector of strings split across ','
			// this takes O(N) time where N is length of string
			// Also note: N is directly proportional to E which is
					// the number of edges for a particular vertex
			while(getline(tokenStream, token, ',')){
				tokens.push_back(token);
			}
			for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it){
				// Get full number in front of " C"
				int chem2 = stoi((*it).substr(2));
				insert_edge(graph, chem1, chem2);
			}
		}
		return graph;
	}
	f.close();
}
