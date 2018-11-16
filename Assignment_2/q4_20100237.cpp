#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
/**************** ALGORITHM DESCRIPTION ***********************
	The code uses the simple assumption that the first order 
	should have the highest probability/length ratio, or in other
	words, it's probability per unit length should be the most.
	This works because each successive probability is being 
	multiplied by a bigger number (the sum of lengths). Another way to 
	look at it is that: initially the length should be as small as 
	possible, and later the probability should be as max as possible.
	This simplifies to just ordering according to p/L
*/
using namespace std;

struct rack{
	int i;
	double l;
	double p;
	double x = p/l;

	bool operator<(const rack& right) const{
		return (x) < (right.x);
	}
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
priority_queue<rack> parse(string filename){
	string line;
	ifstream f(filename);
	getline(f, line);
	vector<string> first_line = split(line, " ");
	int N = stoi(first_line[1]);
	priority_queue<rack> racks;

	getline(f, line);
	vector<string> lengths = split(line, " ");
	getline(f, line);
	vector<string> probs = split(line, " ");
	for (int i = 0; i < N; ++i)	{
		double l = stod(lengths[i+1]);
		double p = stod(probs[i+1]);
		rack temp {i+1, l, p};
		racks.push(temp);
	}

	return racks;
}

// Just need to pop from queue. Takes (logn)
void find_time(priority_queue<rack> racks){
	double total_length = 0;
	double total_time = 0;
	while(!racks.empty()){
		rack temp = racks.top();
		cout << "rack" << temp.i << " " ;
		total_length += temp.l;
		double time = temp.p * total_length;
		total_time += time;
		racks.pop();
	}
	cout << endl << "Expected time: " << total_time << endl;
}

int main(){
	priority_queue<rack> racks = parse("q4  test case 1.txt");

	find_time(racks);
	return 0;
}