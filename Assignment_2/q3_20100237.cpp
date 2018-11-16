#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

struct matrix{
	int* r;
	int* c;
	int N;
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


// Parses and directly puts into a priority queue
// Inserts in the priority queue in O(logn) time
matrix parse(string filename){
	string line;
	ifstream f(filename);
	getline(f, line);
	vector<string> first_line = split(line, " ");
	int N = stoi(first_line[1]);
	bool** wow = NULL;
	int* rows = new int[N];
	int* cols = new int[N];

	getline(f, line);
	vector<string> row_string = split(line, " ");
	getline(f, line);
	vector<string> col_string = split(line, " ");

	for (int i = 0; i < N; ++i)	{
		int r = stoi(row_string[i+1]);
		int c = stoi(col_string[i+1]);
		rows[i] = r;
		cols[i] = c;
	}

	matrix m {rows, cols, N};
	return m;
}

void find_matrix(int* rows,int* cols,int N){
	bool** matrix = new bool*[N];
	for(int i = 0; i < N; i++){
		matrix[i] = new bool[N];
		for(int j = 0; j < N; j++){
			matrix[i][j] = 0;
		}
	}
	for (int i = 0; i < N; ++i)	{
		int max_col = 0;
		while(rows[i] > 0){
			int max_col = 0;
			int max_value = INT8_MIN;
			for(int j = 0; j < N; j++){
				if(cols[j] > max_value && !matrix[i][j]){
					max_value = cols[j];
					max_col = j;
				}
			}
			matrix[i][max_col] = 1;
			rows[i]--;
			cols[max_col]--;
		}
	}
	for(int i = 0; i < N; i++){
		if(rows[i] || cols[i]){
			cout << "NOT POSSIBLE" << endl;
			return;
		}
	}
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			if(matrix[i][j]){
				cout << "(" << i << "," << j << ")";
			}
		}
		cout << endl;
	}
}

int main(){
	matrix mat = parse("q3  test case 2.txt");
	int N = mat.N;
	int* rows = mat.r;
	int* cols = mat.c;

	find_matrix(rows, cols, N);

	return 0;
}