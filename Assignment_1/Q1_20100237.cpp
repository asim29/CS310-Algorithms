#include <iostream>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <queue>

using namespace std;

// function to convert character to int
int ctoi(int c){
	return c - '0';
}

// Using a graph structure with an array of unordered_sets
// An edge between two chemicals represents they react with each other
// since unordered_sets are stored as hash tables in C++
// Adding or searching for an element both take O(1) time in a hash table
struct Graph{
	int V;
	unordered_set<int>* edges;

	Graph(int N) : V(N)
	{
		edges = new unordered_set<int>[N];
	}
};
// Since it is an undirected graph
// just need to add chem1 into the edges list of chem2 and vice-versa
void insert_edge(Graph* graph, int chem1, int chem2){
	graph->edges[chem1].insert(chem2);
	graph->edges[chem2].insert(chem1);
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
			string reacts = line.substr(end+1);

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

string fill_colors(Graph* graph, queue<int> &q, string* colors){
	if (q.empty()){
		return "OK";
	}
	int v = q.front();
	q.pop();

	for(auto itr = graph->edges[v].begin(); itr != graph->edges[v].end(); itr++){
		int u = *itr;
		if (colors[u] == "none"){
			if (colors[v] == "red")
				colors[u] = "blue";
			else
				colors[u] = "red";
			q.push(u);
		} else if (colors[u] == colors[v]) {
			// Two colors not enough, therefore it is not a bipartite graph
			string to_return = to_string(v) + to_string(u);
			return to_return;
		}
	}

	string result = fill_colors(graph, q, colors);
	if (result == "OK")
		return "OK";
	else{
		// The current parent is either reactive with:
		int u_start = ctoi(result[0]);
		int u_end = ctoi(result[result.length()-1]);
		if (check_reactivity(graph, v, u_start))
		// the first element in the result or
			return to_string(v) + result;
		else if (check_reactivity(graph, v, u_end))
		// the last element in the result or
			return result + to_string(v);
		else
		// not reactive with either i.e. not a part of cycle
			return result;
	}
}

void print_pack(vector<int> pack){
		for(int i = 0; i < pack.size()-1; i++){
			cout << "C" << pack[i] << ", ";
		}
		cout << "C" << pack[pack.size()-1] << endl;
}

void print_cycle(string cycle){
	for(int i = 0; i < cycle.length(); i++){
		cout << "C" << cycle[i] << "->";
	}
	cout << cycle[0] << endl;
}

void can_pack(Graph* graph){
	queue<int> q;
	string* colors = new string[graph->V];
	for(int i = 0; i < graph->V; i++){
		colors[i] = "none";
	}

	q.push(0);
	colors[0] = "red";

	string result = fill_colors(graph, q, colors);
	if(result == "OK"){
		cout << "Yes" << endl;
		vector<int> pack1;
		vector<int> pack2;
		for(int i = 0; i < graph->V; i++){
			if(colors[i] == "red"){
				pack1.push_back(i);
			} else {
				pack2.push_back(i);
			}
		}
		print_pack(pack1);
		print_pack(pack2);
	} else {
		cout << "No" << endl;
		print_cycle(result);
	}
	
}

int main(){
	Graph* graph = parse_file("Q1_input4.txt");
	can_pack(graph);
	return 0;
}