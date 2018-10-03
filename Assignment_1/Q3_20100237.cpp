#include <iostream>
#include <unordered_set>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>
#include <queue>

using namespace std; 
/* 	============================ ALGORITHM DESCRIPTION ============================
	A simple Dijsktra algorithm is being implemented with time complexity O(|V|+|E|)
				and space the same as before due to adjacency list
*/
struct vertex { 
   int key; 
   int dist;  
}; 
  
// To compare two vertices based on distance 
struct comparator { 
    int operator() (const vertex& u, const vertex& c){ 
        return u.dist > c.dist; 
    } 
}; 

struct Graph{
	int V;
	vector<vertex>* in_edges;
	vector<vertex>* out_edges;

	Graph(int N) : V(N)
	{
		in_edges = new vector<vertex>[N];
		out_edges = new vector<vertex>[N];
	}
};

void insert_edge(Graph* graph, int u, int v, int dist){
	vertex V({v, dist});
	vertex U({u, dist});
	graph->out_edges[U.key].push_back(V);
	graph->in_edges[V.key].push_back(U);
}

void print_graph(const Graph* graph){
	for(int i = 0; i < graph->V; i++){
		cout << "C" << i << ": ";
		for(auto itr = graph->out_edges[i].begin(); itr != graph->out_edges[i].end(); itr++){
			cout << "C" << (*itr).key << ";" << (*itr).dist << ", ";
		}
		cout << endl;
	}
}

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
			int u = stoi(line.substr(0,end).substr(1));
			// find right side numbers
			// cout << "U" << u << ": ";
			string neighbours = line.substr(end+1);

			vector<string> tokens;
			string token;
			if(neighbours.length() > 0){
				istringstream tokenStream(neighbours);
				// find vector of strings split across ','
				// this takes O(N) time where N is length of string
				// Also note: N is directly proportional to E which is
						// the number of edges for a particular vertex
				while(getline(tokenStream, token, ',')){
					tokens.push_back(token);
				}
				for(vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it){
					// Get full number in front of " C"
					string node = (*it).substr(2);
					// split across ";"
					int div = node.find(";");
					int v = stoi(node.substr(0,div));
					int dist = stoi(node.substr(div+1));
					insert_edge(graph, u, v, dist);
				}				
			}
		}
		return graph;
	}
	f.close();
}

void printPath(int dest, int* parent){
	if(parent[dest] == -1)
		return;

	printPath(parent[dest], parent);
	cout << "->C" << dest  ;
}

// This will find the shortest distance from u to all vertices
int** dijkstra(struct Graph* graph, int start){
	int* dist = new int[graph->V];
	int* parents = new int[graph->V];

	priority_queue<vertex, vector<vertex>, comparator> pq;
	for(int i = 0; i < graph->V; i++){
		parents[i] = -1;
		dist[i] = INT8_MAX;
		if (i != start ){
			vertex v({i, dist[i]});
			pq.push(v);
		}
	}
	dist[start] = 0;
	vertex start_point({start, 0});
	pq.push(start_point);

	while(pq.size()){
		vertex min_vertex = pq.top();
		pq.pop();
		int u = min_vertex.key;
		for(auto itr = graph->out_edges[u].begin(); itr != graph->out_edges[u].end(); itr++){
			int v = (*itr).key;
			int weight = (*itr).dist;
			if(dist[v] > dist[u] + weight){
				parents[v] = u;
				dist[v] = dist[u] + weight;
				vertex vx({v, dist[v]});
			}
		}
	}
	int** ret = new int*[2];
	ret[0] = dist;
	ret[1] = parents;
	return ret;
}

void find_best_path(Graph* graph, int home, int frnd, int scenic){
	int** from_home = dijkstra(graph, home);
	int* dist_from_home = from_home[0];
	int* parents_from_home = from_home[1];
	int** from_scene = dijkstra(graph, scenic);
	int* dist_from_scene = from_scene[0];
	int* parents_from_scene = from_scene[1];

	cout << "Distance from home to friend's house: " << dist_from_home[frnd] << " using path: C" << home;
	printPath(frnd, parents_from_home);
	cout << endl;
	cout << "Distance from home to scenic view: " << dist_from_home[scenic] << " using path: C" << home;
	printPath(scenic, parents_from_home);
	cout << endl;
	cout << "Distance from scenic view to friend's house: " << dist_from_scene[frnd] << " using path: C" << scenic;
	printPath(frnd, parents_from_scene);
	cout << endl;
}
// Driver code 
int main (){
	Graph* graph = parse_file("Q3_input1.txt");
	int home, frnd, scenic;
	cout << "Input home: ";
	// cin >> home;
	// home = 0;
	cout << endl << "Input frnd: ";
	// cin >> frnd;
	// frnd = 2;
	cout << endl << "Input scenic: ";
	// cin >> scenic;
	// scenic = 3;
	cout << endl;

	find_best_path(graph, home, frnd, scenic);
	// int* shortest_paths = dijkstra(graph, 0);
	// for(int i = 0; i < graph->V; i++){
		// cout << "C" << i << " distance: " << shortest_paths[i] << endl;
	// }
  
    return 0; 
} 