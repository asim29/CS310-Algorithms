#include "../graph.cpp"
#include <queue>
#include <set>

using namespace std;

/* 	============================ ALGORITHM DESCRIPTION ============================
	For this entire question I am reusing the same graph structure as in Q1, so search
	and insertion is all linear time.

	For part a: I am using the fact that in a directed acyclic graph, there will be at
	least one node with in-degree 0, as explained by ma'am in class. Using this fact I 
	add all those initial vertex with in-degree 0 in a queue at the start, and then put 
	the queue into a recursive algorithm. This pops the vertex from the queue, adds it
	into the topological sort and then subtracts the in-degrees of all neighbouring 
	vertices, and then checks if any in-degree has become 0, if it has, push it into the
	queue. If the algorithm stops before all the vertices have been added into the topo
	sort, that means there was a cycle inside which starts from the last node that has 
	been added. We use that node to find a cycle using a simple DFS traversal, similar
	to the color problem in part 1. This entire algorithm, since it only consists of
	traversal over the graph's vertexes, is O(|V| + |E|) in both time and space complexity

	For part b: I am doing the same thing, except I have made a struct in which I maintain
	what level the current traversal is at. All the nodes at the same level can be put
	into the same row

*/

struct vertex_level{
	int vertex;
	int level;
};

// This function is given the initial queue with only the elements with zero in-degree
// It puts those first in the topological order, and then subtracts the in-degree of all
// of its neighbours, and if any in-degree becomes zero, it pushes it into the queue. 
// It then calls itself recursively on the newly calculated in-degrees
void recur_topo(Graph* graph, int* in_degrees, queue<int> &q, queue<int> &topo_order){
	static int visited = 0;
	if(q.empty()){
		return;
	}

	int u = q.front();
	q.pop();
	topo_order.push(u);
	visited++;
	// Decrease the in-degree of all neighbours by 1
	for(auto itr = graph->out_edges[u].begin(); itr != graph->out_edges[u].end(); itr++){
		int v = *itr;
		in_degrees[v]--;
		if(in_degrees[v] == 0){
			q.push(v);
		}
	}

	recur_topo(graph, in_degrees, q, topo_order);
}

queue<int> topo_order(Graph* graph){
	queue<int> q;
	queue<int> topo_order;
	int* in_degrees = new int[graph->V];
	for(int i = 0; i < graph->V; i++){
		in_degrees[i] = find_indegree(graph, i);
	}
	int i = 0;
	for(i = 0; i < graph->V; i++){
		if(in_degrees[i] == 0){
			q.push(i);
		}
	}

	recur_topo(graph, in_degrees, q, topo_order);

	// cout << "Returning with size " << topo_order.size() << endl;

	return topo_order;
}

void recur_topo_row(Graph* graph, int* in_degrees, queue<vertex_level> &q, vector<vertex_level> &row_order){
	static int visited = 0;
	if(q.empty()){
		return;
	}

	vertex_level u = q.front();
	q.pop();
	int u_val = u.vertex;
	row_order.push_back(u);
	visited++;
	for(auto itr = graph->out_edges[u_val].begin(); itr != graph->out_edges[u_val].end(); itr++){
		int v_val = *itr;
		in_degrees[v_val]--;
		if(in_degrees[v_val] == 0){
			int v_level = u.level + 1;
			vertex_level v({v_val, v_level});
			q.push(v);
		}
	}

	recur_topo_row(graph, in_degrees, q, row_order);	
}

vector<vertex_level> topo_row_order(Graph* graph){
	queue<vertex_level> q;
	vector<vertex_level> row_order;

	int* in_degrees = new int[graph->V];
	for(int i = 0; i < graph->V; i++){
		in_degrees[i] = find_indegree(graph, i);
	}
	for(int i = 0; i < graph->V; i++){
		if(in_degrees[i] == 0){
			// cout << "Initially putting " << i << endl;
			vertex_level v({i, 0});
			q.push(v);
		}
	}

	recur_topo_row(graph, in_degrees, q, row_order);

	return row_order;
}
// A similar function to the color problem done in Q1 except the color "none" 
// represents not visited, "red" represents that the DFS on a node is completed 
// and "blue" represents that the DFS on the node is currently taking place
// This function takes a total of O(|V|+|E|) time
string get_cycle(Graph* graph, int u, string* colors){
	colors[u] = "blue";
	for(auto itr = graph->out_edges[u].begin(); itr != graph->out_edges[u].end(); itr++){
		int v = *itr;
		if (colors[v] == "blue"){
			return to_string(v);
		}

		else if(colors[v] == "none"){
			string result = get_cycle(graph, v, colors);
			if(result != "OK"){
				return to_string(v)+result;
			}
		}
	}

	colors[u] = "red";
	return "OK";
}

int main(){

	Graph* graph = parse_file("Q2_input3.txt");

	// print_graph(graph);
	cout << "Finding single line order for Q 2a: " << endl;
	queue<int> q = topo_order(graph);

	if(q.size() != graph->V){
		cout << "Topological ordering not possible, there is a cycle" << endl;
		int u = 0;
		while(!q.empty()){
			u = q.front();
			q.pop();
		}
		string* colors = new string[graph->V];
		for(int i = 0; i < graph->V; i++){
			colors[i] = "none";
		}
		string cycle = to_string(u) + get_cycle(graph, u, colors);
		cout << "L" << cycle[0];
		for(int i = 1; i < cycle.length(); i++){
			cout << "->L" << cycle[i];
		}
		cout << endl;
	}
	else{
		cout << "Yes" << endl;
		cout << "L" << q.front();
		q.pop();
		while(!q.empty()){
			cout << ", L" << q.front();
			q.pop();
		}		
	}
	cout << endl << endl;

	cout << "Finding row order for Q 2b: " << endl;

	vector<vertex_level> answer = topo_row_order(graph);
	if(answer.size() == graph->V){
		vector<int>* final_vec = new vector<int>[graph->V];
		for(auto itr = answer.begin(); itr != answer.end(); itr++){
			vertex_level u = *itr;
			final_vec[u.level].push_back(u.vertex);
		}
		for(int i = 0; i < graph->V; i++){
			if(!final_vec[i].empty()){
				auto itr = final_vec[i].begin();
				cout << "L" << *itr++;
				for(itr; itr != final_vec[i].end(); itr++){
					cout << ", L" << *itr;
				}
				cout << endl;
			}
		}		
	} else {
		cout << "Topological ordering not possible, there is a cycle" << endl;
		vertex_level u({0,0});
		for(auto itr = answer.begin(); itr != answer.end(); itr++){
			u = *itr;
		}
		string* colors = new string[graph->V];
		for(int i = 0; i < graph->V; i++){
			colors[i] = "none";
		}
		string cycle = to_string(u.vertex) + get_cycle(graph, u.vertex, colors);
		cout << "L" << cycle[0];
		for(int i = 1; i < cycle.length(); i++){
			cout << "->L" << cycle[i];
		}
		cout << endl;	}

	return 0;
}