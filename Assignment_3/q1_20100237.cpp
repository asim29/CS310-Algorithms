#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
/******************************* ALGORITHM DESCRIPTION *******************************
The code works by implementing the Gale-Shapley algorithm as stated in the book. It picks
up the first free airport (x) it sees and tries to assign a plane to it, and then sets it
as "assigned" i.e. no longer free. Later if another airplane (y) wants to get that plane, 
and that particular plane also prefers y, then the plane is reassigned to y  and x is made 
free again. This iterative process continues until there are no free airports. Since every
 airport checks every plane, the entire algorithm takes O(n^2) time where n is number of 
 airports/planes. One more case in which it can terminate is when no stable matching exists 
 (which can happen because of set X), in this case it is able to terminate by checking 
 whether it has entered an infinite loop in which none of the airports are ever free - 
 								and thus it terminates
*************************************************************************************/
struct preferences{
	int N;
	vector<int>* plane_lists;
	vector<int>* airport_lists;
	bool** allowed;
};

// Custom split function that splits according to arbitrary delimiter
// And returns a vector of split strings
vector<string> split(string s, string delimiter){
	// cout << s << delimiter << endl;
	size_t pos = 0;
	string token;
	vector<string> split_string;
	while ((pos = s.find(delimiter)) < string::npos) {
	    token = s.substr(0, pos);
	    split_string.push_back(token);
	    s.erase(0, pos + delimiter.length());
	}
	split_string.push_back(s);

	return split_string;
}
	
template <class T>
void print_vec(vector<T> vec){
	for(auto itr = vec.begin(); itr < vec.end(); itr++){
		cout << *itr << " ";
	}
	cout << endl;
}

struct preferences* parse(string filename){
	cout << filename << endl;
	preferences* all_lists = new preferences;
	string line;
	ifstream f(filename);
	getline(f, line);
	vector<string> split_line = split(line, " ");
	int N = stoi(split_line[1]);

	vector<int>* plane_lists = new vector<int>[N];
	vector<int>* airport_lists = new vector<int>[N];
	bool** allowed = new bool*[N];
	for (int i = 0; i < N; ++i){
		allowed[i] = new bool[N];
		for(int j = 0; j < N; ++j){
			allowed[i][j] = true;
		}	
	}

	for(int i = 0; i < N; i++){
		getline(f, line);
		split_line = split(line, ": ");
		line = split_line[1];
		split_line = split(line, ",");
		for (int j = 0; j < N; ++j)
		{
			vector<string> num = split(split_line[j], "A");
			// Changing from 1-indexed to 0-indexed
			plane_lists[i].push_back(stoi(num[1])-1);
		}
		// print_vec(plane_lists[i]);
	}
	// cout << endl;
	for(int i = 0; i < N; i++){
		getline(f, line);
		split_line = split(line, ": ");
		line = split_line[1];
		split_line = split(line, ",");
		for (int j = 0; j < N; ++j)
		{
			vector<string> num = split(split_line[j], "P");
			// Changing from 1-indexed to 0-indexed
			airport_lists[i].push_back(stoi(num[1])-1);
		}
		// print_vec(airport_lists[i]);
	}
	// cout << endl;
	getline(f, line);
	split_line = split(line, ": ");
	split_line = split(split_line[1], ", ");
	for(int i = 0; i < split_line.size(); i++){
		// Extract the number in front of P, taking care of double digits
		int p_num = stoi(split(split_line[i], "P")[1]);
		int a_num = stoi(split(split_line[i], "A")[1]);
		allowed[p_num-1][a_num-1] = false;
	} 

	// for (int i = 0; i < N; ++i){
	// 	for (int j = 0; j < N; ++j){
	// 		if(allowed[i][j] == false)
	// 			cout << i << " " << j << endl;
	// 	}
	// }

	all_lists->N = N;
	all_lists->airport_lists = airport_lists;
	all_lists->plane_lists = plane_lists;
	all_lists->allowed = allowed;

	return all_lists;
}

int find_free(bool* free, int N){
	for(int i = 0; i < N; i++){
		if(free[i])
			return i;
	}
	return -1;
}

// Tells whether the plane prefers airport1 over airport2
bool prefers(vector<int>* plane_lists, int plane, int airport1, int airport2, int N){
	vector<int> list = plane_lists[plane];
	for(int i = 0; i < N; i++){
		if(list[i] == airport1){
			return true;
		}
		if(list[i] == airport2){
			return false;
		}
	}
}

int* stable_matching(preferences* all_lists){
	int N = all_lists->N;
	vector<int>* airport_lists = all_lists->airport_lists;
	vector<int>* plane_lists = all_lists->plane_lists;
	bool** allowed = all_lists->allowed;

	// The ith plane is in map[i]th airport 
	int* map = new int[N];
	for(int i = 0; i < N; i++){
		map[i] = -1;
	} 

	bool* free = new bool[N];

	// the ith airport is free
	for(int i = 0; i < N; i++){
		free[i] = true;
	}
	int i = 0;
	int c = 0;
	// This while loop is O(n^2)
	while(true){
		int airport = find_free(free, N);
		if(c >= 100*N*N)
			return NULL;
		if(airport == -1)
			break;

		for(int i = 0; i < N && free[airport]; i++){
			int plane = airport_lists[airport][i];
			if(map[plane] == -1 && allowed[plane][airport]){
				map[plane] = airport;
				free[airport] = false;
			} else {
				int other_airport = map[plane];
				if(prefers(plane_lists, plane, airport, other_airport, N) && allowed[plane][airport]){
					free[other_airport] = true;
					map[plane] = airport;
					free[airport] = false;
				}
			}
		}
		c++;
	}

	return map;
}

int main(int argc, char const *argv[])
{
	preferences* pref;
	string file = "q1_test_case1.txt";
	cout << "Enter filename: " << endl;
	cin >> file;
	pref = parse(file);
	int* ans = stable_matching(pref);

	if(ans == NULL){
		cout << "No stable matching possible" << endl;
		return 0;
	}
	int* output = new int[pref->N];
	for(int i = 0; i < pref->N; i++){
		for(int j = 0; j < pref->N; j++){
			if(ans[j] == i){
				output[i] = j;
			}
		}
	}
	cout << "Final Destinations: ";
	for (int i = 0; i < pref->N; ++i)
	{
		cout << "A" << i+1 << " P" << output[i]+1 << ", ";
	}
	cout << endl;

	return 0;
}