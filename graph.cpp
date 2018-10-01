#include <iostream>
#include <unordered_set>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>

using namespace std;
// Using a graph structure with an array of unordered_sets
// An edge between two chemicals represents they react with each other
// since unordered_sets are stored as hash tables in C++
// Adding or searching for an element both take O(1) time in a set
struct Graph{
	int V;
	unordered_set<int>* in_edges;
	unordered_set<int>* out_edges;

	Graph(int N) : V(N)
	{
		in_edges = new unordered_set<int>[N];
		out_edges = new unordered_set<int>[N];
	}
};
// Assume directed graph: add u -> v in two arrays
// the edge is an out-edge for u and and in-edge for v
void insert_edge(Graph* graph, int u, int v){
	graph->out_edges[u].insert(v);
	graph->in_edges[v].insert(u);
}

// This function takes on average O(1) time since sets are stored as hash tables
// But in worst case it will take O(|V| + |E|) time, when:
	// there is an edge from each vertex to every other vertex
bool check_edge(const Graph* graph, int u, int v){
	auto itr = graph->out_edges[u].find(v);

	itr != graph->out_edges[u].end() ? true : false;
}

void print_graph(Graph* graph){
	for(int i = 0; i < graph->V; i++){
		cout << "L" << i << ": ";
		for(auto itr = graph->out_edges[i].begin(); itr != graph->out_edges[i].end(); itr++){
			cout << "L" << *itr << ", ";
		}
		cout << endl;
	}
}

// This is done in O(1)
// ref: http://www.cplusplus.com/reference/unordered_set/unordered_set/size/
int find_indegree(const Graph* graph, int u){
	return graph->in_edges[u].size();
}

int find_outdegree(const Graph* graph, int u){
	return graph->out_edges[u].size();
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
			if(reacts.length() > 0){
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

		}
		return graph;
	}
	f.close();
}
// function to convert character to int
int ctoi(int c){
	return c - '0';
}