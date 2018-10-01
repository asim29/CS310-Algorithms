#include <vector>
#include <queue>
#include "../graph.cpp"

using namespace std;

/* 	======================== ALGORITHM DESCRIPTION ========================
	The problem is being broken into finding an odd-length cycle in a 
	graph where each vertex in the graph is a chemical while each edge 
	represents that the two chemicals can react. The odd-length	cycles are
	found by coloring the vertices into two colors using BFS, which takes
	O(|V| + |E|) time. If there are no cycles then BFS is run only once
	and the two colored vertices are printed separately. If by the initial 
	BFS we find a cycle, the node that is part of the cycle is returned and
	another BFS is run on that node to find the full path of the cycle.

	Running BFS twice is still O(|V| + |E|)

	Overall Running Time Complexity: O(|V|+|E|).
	Overall Space Complexity (Because of adjacency list): O(|V| + |E|)
*/

// This function simply runs BFS, which is O(|V| + |E|) on any given node 
	// and returns one of two things
// 1. "OK" means that there is no odd-length cycle all vertices have been colors appropriately
// 2. Anything else means that the number returned is the problem node
string fill_colors(const Graph* graph, queue<int> &q, string* colors){
	if (q.empty()){
		return "OK";
	}
	int v = q.front();
	q.pop();

	// color every child of v a different color from v and push into queue
	for(auto itr = graph->in_edges[v].begin(); itr != graph->in_edges[v].end(); itr++){
		int u = *itr;
		if (colors[u] == "none"){
			if (colors[v] == "red")
				colors[u] = "blue";
			else
				colors[u] = "red";
			q.push(u);
		} else if (colors[u] == colors[v]) {
			// Two colors not enough, therefore it is not a bipartite graph
			return to_string(u);
		}
	}

	return fill_colors(graph, q, colors);
}

// This does a similar thing to the previous function except:
	// It is only called once a cycle is confirmed, and a BFS is run
		// on the problem node to find the path of that cycle
string find_cycle(const Graph* graph, queue<int> &q, string* colors){
	if (q.empty())
		return "OK";

	int v = q.front();
	q.pop();

	for(auto itr = graph->in_edges[v].begin(); itr != graph->in_edges[v].end(); itr++){
		int u = *itr;
		if(colors[u] == "none"){
			if (colors[v] == "red")
				colors[u] = "blue";
			else
				colors[u] = "red";
			q.push(u);
		} else if (colors[u] == colors[v]){
			// start to return full path
			string to_return = to_string(v) + to_string(u);
			return to_return;
		}
	}
	string result = find_cycle(graph, q, colors);
	if(result == "OK")
		return "OK";
	else{
		// The current parent is either reactive with:
		int u_start = ctoi(result[0]);
		int u_end = ctoi(result[result.length()-1]);
		if (check_edge(graph, v, u_start))
		// the first element in the result or
			return to_string(v) + result;
		else if (check_edge(graph, v, u_end))
		// the last element in the result or
			return result + to_string(v);
		else
		// not reactive with either i.e. not a part of cycle
			return result;
	}
}

void print_box(vector<int> box){
		for(int i = 0; i < box.size()-1; i++){
			cout << "C" << box[i] << ", ";
		}
		cout << "C" << box[box.size()-1] << endl;
}

void print_cycle(string cycle){
	for(int i = 0; i < cycle.length(); i++){
		cout << "C" << cycle[i] << "->";
	}
	cout << "C" << cycle[0] << endl;
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
		vector<int> box1;
		vector<int> box2;
		for(int i = 0; i < graph->V; i++){
			if(colors[i] == "red"){
				box1.push_back(i);
			} else {
				box2.push_back(i);
			}
		}
		print_box(box1);
		print_box(box2);
	} else {
		cout << "No" << endl;
		// get problem node
		int cycle_chem = stoi(result);
		// empty the queue
		while(!q.empty()){
			q.pop();
		}
		// reinitialize colors array
		for(int i = 0; i < graph->V; i++){
			colors[i] = "none"; 
		}
		// run BFS on problem node
		q.push(cycle_chem);
		colors[cycle_chem] = "red";
		result = find_cycle(graph, q, colors);
		print_cycle(result);
	}
	
}

int main(){
	Graph* graph = parse_file("Q1_input1.txt");
	can_pack(graph);
	return 0;
}