#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>

using namespace std;
/******************************* ALGORITHM DESCRIPTION *******************************
The base case for this would be a matrix of size 2 in which there is one blank square.
To fill that you just need to fill the rest of the three squares. For the rest of the
cases you need to first fill the center of the matrix in such a way that each submatrix
of size N/2 has just one blank/filled square. So the subproblem is identical for all
submatrices. And then send those submatrices recursively into the function. The recurrence
relation for this is as follows:
T(n) = 4T(n/2) + C where C is a constant. The algorithm takes O(nlogn) time and takes
O(n^2) space.
*************************************************************************************/
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

int* make_matrix(char** &matrix, string filename){
	int* point = new int[3];
	string line;
	ifstream f(filename);
	getline(f, line);
	vector<string> split_line = split(line, " ");
	int N = stoi(split_line[1]);

	getline(f, line);
	split_line = split(line, " ");
	int r = stoi(split_line[1]);

	getline(f, line);
	split_line = split(line, " ");
	int c = stoi(split_line[1]);

 	matrix = new char*[N];
 	for(int i = 0; i < N; i++){
 		matrix[i] = new char[N];
 		for(int j = 0; j < N; j++){
 			// if(i >= N/2){
 			// 	if(j >= N/2)
 			// 		matrix[i][j] = '%';
 			// 	else
		 	// 		matrix[i][j] = '!';
 			// }
 			// else{
 			// 	if(j >= N/2)
	 		// 		matrix[i][j] = '?';
	 		// 	else
	 		// 		matrix[i][j] = '#';
 			// }
 			matrix[i][j] = '?';
 		}
 	}

 	point[0] = N;
 	point[1] = r;
 	point[2] = c;

 	return point;
}

void fill_matrix(char** matrix, int N, int start_i, int start_j, int point_i, int point_j, queue<char>* chars){
	if(N == 2){
		char c = chars->front();
		chars->pop();
		for(int i = start_i; i < N+start_i; i++){
			for(int j = start_j; j < N+start_j; j++){
				if(matrix[i][j] == '?'){
					matrix[i][j] = c;
				}
			}
		}
	} else {
		char c = chars->front();
		chars->pop();
		int center_i = start_i+N/2;
		int center_j = start_j+N/2;
		if(point_i < center_i){
			if(point_j < center_j){
				matrix[center_i-1][center_j] = c;
				matrix[center_i][center_j-1] = c;
				matrix[center_i][center_j] = c;
			} else{
				matrix[center_i-1][center_j-1] = c;
				matrix[center_i][center_j-1] = c;
				matrix[center_i][center_j] = c;
			}
		} else {
			if(point_j < center_j){
				matrix[center_i-1][center_j-1] = c;
				matrix[center_i-1][center_j] = c;
				matrix[center_i][center_j] = c;
			} else {
				matrix[center_i-1][center_j-1] = c;
				matrix[center_i-1][center_j] = c;				
				matrix[center_i][center_j-1] = c;
			}
		}
		int new_N = N/2;
		fill_matrix(matrix, new_N, start_i, start_j, point_i, point_j, chars);
		fill_matrix(matrix, new_N, start_i, center_j, point_i, point_j, chars);
		fill_matrix(matrix, new_N, center_i, start_j, point_i, point_j, chars);
		fill_matrix(matrix, new_N, center_i, center_j, point_i, point_j, chars);
	}
}

void print_matrix(char** matrix, int N, int start_r, int start_c){
	for(int i = start_r; i < N+start_r; i++){
		for(int j = start_c; j < N+start_c; j++){
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

int main(int argc, char const *argv[])
{
	char** matrix;
	string filename = "test_cases/q3_test_case.txt";
	// cout << "Enter filename: " << endl;
	// cin >> file;
	int* point = make_matrix(matrix, filename);
	int N = point[0];
	int r = point[1];
	int c = point[2];
	matrix[r][c] = '0';

	queue<char>* chars = new queue<char>;
	for(char c = 'A'; c < 'z'; c++){
		chars->push(c);
	}

	// fill_matrix(matrix, N, chars);
	// print_matrix(matrix, N, 0, 0);

	fill_matrix(matrix, N, 0, 0, r, c, chars);

	cout << endl; 
	print_matrix(matrix, N, 0, 0);


	return 0;
}
