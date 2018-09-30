#include <iostream>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;


// Using a graph structure with an array of unordered_sets
// An edge between two chemicals represents they react with each other
// since unordered_sets are stored as hash tables in C++
// Adding or searching for an element both take O(1) time in a hash table
struct Graph{
	int V;
	unordered_set<int>* edges;

	Graph(int N) : V(N)
	{
		cout << "Creating graph of size: " << V << endl;
		edges = new unordered_set<int>[N];
	}
};

// Since it is an undirected graph
// just need to add chem1 into the edges list of chem2 and vice-versa
void insert_edge(Graph* graph, int chem1, int chem2){
	graph->edges[chem1].insert(chem2);
	graph->edges[chem2].insert(chem1);
}

void print_graph(const Graph* graph){
	for(int i = 0; i < graph->V; i++){
		cout << "C" << i << ": ";
		for(auto itr = graph->edges[i].begin(); itr != graph->edges[i].end(); itr++){
			cout << "C" << *itr << ", ";
		}
		cout << endl;
	}
}

bool check_reactivity(const Graph* graph, int chem1, int chem2){
	auto itr = graph->edges[chem1].find(chem2);

	itr != graph->edges[chem1].end() ? true : false;
}



Graph* parse_file(string filename){
	string line;
	ifstream f(filename);
	if(f.is_open()){
		getline(f, line);
		int n = stoi(line.substr(2));

		Graph* graph = new Graph(n);
		while(getline(f, line)){
			// find position of :
			int end = line.find(":");

			// find initial number
			int chem1 = stoi(line.substr(0,end).substr(1));
			// cout << "C" << chem1;
			string reacts = line.substr(end+1);
			// cout << reacts;

			vector<string> tokens;
			string token;
			istringstream tokenStream(reacts);
			while(getline(tokenStream, token, ',')){
				tokens.push_back(token);
			}
			for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it){
				int chem2 = stoi((*it).substr(2));

				insert_edge(graph, chem1, chem2);
			}

		}
		return graph;
	}
	f.close();
}

vector<int>* check_bipartite(graph){
	
}

int main(){
	Graph* graph = parse_file("Q1_input2.txt");
	print_graph(graph);
	return 0;
}