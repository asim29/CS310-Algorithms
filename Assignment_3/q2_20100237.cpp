#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

/******************************* ALGORITHM DESCRIPTION *******************************
The algorithm is exactly the same as Q1 but the way the preference lists are set up is
slightly different. For the teams - the location at which a team first arrives is set
to highest priority, and the location at which it arrives at the end is the least priority.
For Locations - the team which arrives at that location at the end is the highest preferred
team by that location, and the team which arrives first at that location is the lowest
preferred team by that location. The rest of the algo is the same and is hence O(n^2)
*************************************************************************************/
struct preferences{
	int** matrix;
	vector<int>* loc_list;
	vector<int>* team_list;
	int N;
	int M;
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
		cout << *itr << endl;
	}
	cout << endl;
}

preferences* parse(string filename){
	string line;
	ifstream f(filename);
	getline(f, line);
	vector<string> split_line = split(line, " ");
	int N = stoi(split_line[1]);
	f.close();
	int** matrix = new int*[N];
	f.open(filename);
	getline(f,line);
	getline(f,line);
	split_line = split(line,": ");
	line = split_line[1];
	split_line = split(line, ",");
	int M = split_line.size();
	for(int i = 0; i < N; i++){
		matrix[i] = new int[M];
	}

	for(int i = 0; i < M; i++){
		string word = split_line[i];
		if(word[0] == '-')
			matrix[0][i] = -1;
		else{
			vector<string> split_word = split(word, "L");
			int num = stoi(split_word[1]);
			// change indexes to start from 0 instead of 1
			matrix[0][i] = num-1;
		} 
	}
	for(int i = 1; i < N; i++){
		getline(f,line);
		split_line = split(line,": ");
		line = split_line[1];
		split_line = split(line, ",");
		for(int j = 0; j < M; j++){
			string word = split_line[j];
			if(word[0] == '-')
				matrix[i][j] = -1;
			else{
				vector<string> split_word = split(word, "L");
				int num = stoi(split_word[1]);
				// change indexes to start from 0 instead of 1
				matrix[i][j] = num-1;
			}			
		}
	}

	// for(int i = 0; i < N; i++){
	// 	cout << "T" << i << ": ";
	// 	for(int j = 0; j < M; j++){
	// 		cout << matrix[i][j] << ", ";
	// 	}
	// 	cout << endl;
	// }

	preferences* ret = new preferences;
	ret->matrix = matrix;
	ret->N = N;
	ret->M = M;
	return ret;
}

preferences* make_lists(preferences* pref){
	int** matrix = pref->matrix;
	int N = pref->N;
	int M = pref->M;

	vector<int>* loc_list = new vector<int>[N];
	vector<int>* team_list = new vector<int>[N];

	for(int i = M-1; i >= 0; i--){
		for(int j = 0; j < N; j++){
			int loc = matrix[j][i];
			if(loc != -1){
				loc_list[loc].push_back(j);
			} 
		}
	}
	for(int i = 0; i < N; i++){
		for(int j = 0; j < M; j++){
			int loc = matrix[i][j];
			if(loc != -1){
				team_list[i].push_back(loc);
			}
		}
	}

	// for(int i = 0; i < N; i++){
	// 	cout << "L" << i << ": ";
	// 	for(auto itr = loc_list[i].begin(); itr != loc_list[i].end(); itr++){
	// 		cout << *itr << ", ";
	// 	}
	// 	cout << endl;	
	// }
	pref->loc_list = loc_list;
	pref->team_list = team_list;
}

int find_free(bool* free, int N){
	for(int i = 0; i < N; i++){
		if(free[i])
			return i;
	}
	return -1;
}

bool prefers(vector<int>* team_lists, int team, int loc1, int loc2, int N){
	vector<int> list = team_lists[team];
	for(int i = 0; i < N; i++){
		if(list[i] == loc1){
			return true;
		}
		if(list[i] == loc2){
			return false;
		}
	}
}

int* stable_matching(preferences* all_lists){
	int N = all_lists->N;
	vector<int>* loc_lists = all_lists->loc_list;
	vector<int>* team_lists = all_lists->team_list;

	// The ith team is in map[i]th loc 
	int* map = new int[N];
	for(int i = 0; i < N; i++){
		map[i] = -1;
	} 

	bool* free = new bool[N];

	// the ith loc is free
	for(int i = 0; i < N; i++){
		free[i] = true;
	}
	int i = 0;
	while(true){
		int loc = find_free(free, N);
		if(loc == -1)
			break;

		for(int i = 0; i < N && free[loc]; i++){
			int team = loc_lists[loc][i];
			if(map[team] == -1){
				map[team] = loc;
				free[loc] = false;
			} else {
				int other_loc = map[team];
				if(prefers(team_lists, team, loc, other_loc, N)){
					free[other_loc] = true;
					map[team] = loc;
					free[loc] = false;
				}
			}
		}
	}

	return map;
}
	
int main(int argc, char const *argv[])
{
	preferences* matrix;
	string filename = "test_cases/q2_test_case.txt";
	// cout << "Enter filename: " << endl;
	// cin >> filename;
	matrix = parse(filename);	
	matrix = make_lists(matrix);
	int* ans = stable_matching(matrix);

	int* output = new int[matrix->N];
	for(int i = 0; i < matrix->N; i++){
		for(int j = 0; j < matrix->N; j++){
			if(ans[j] == i){
				output[i] = j;
			}
		}
	}
	cout << "Stable Matching: ";
	for (int i = 0; i < matrix->N; ++i)
	{
		cout << "L" << i+1 << " T" << output[i]+1 << ", ";
	}
	cout << endl;

	return 0;
}