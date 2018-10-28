#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_set>

using namespace std;

struct clause{
	int right;
	list<int>* left;

	void print(){
		cout << "Clause: ";
		auto itr = left->begin();
		if(itr != left->end()){
			cout << *itr;
			itr++;
			for(itr; itr != left->end(); itr++){
				cout << "^" << *itr;
			}
		}
		cout << " => " << right << endl;
	}
	int is_singleton(){
		if(!left->size()){
			return right;
		} else {
			return -1;
		}
	}
	bool remove(int i){
		for(auto itr = left->begin(); itr != left->end(); itr++){
			if(i == *itr){
				left->erase(itr);
				itr--;
				return 1;
			}
		}
		return 0;
	}
};

struct horn_formula{
	int size;
	bool* variables;
	list<clause>* implications;
	list<list<int>>* negations; 

	void print(){
		cout << "The assignments: " << endl;
		for(int i = 1; i < size; i++){
			cout << i << ": " << variables[i] << endl;
		}
		cout << "implications: " << endl;
		for(auto itr = implications->begin(); itr != implications->end(); itr++){
			(*itr).print();
		}
		cout << "Negations: " << endl;
		for(auto itr = negations->begin(); itr != negations->end(); itr++){
			cout << "Neg is: ";
			list<int> neg = *itr;
			for(auto i = neg.begin(); i != neg.end(); i++){
				cout << *i << " ";
			}
			cout << endl;
		}
	}

	void print_literals(){
		for(int i = 1; i < size; i++){
			cout << i << ": " << variables[i] << endl;
		}
	}
};

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

horn_formula* parse(string filename){
	string line;
	ifstream f(filename);
	getline(f, line);
	vector<string> first_line = split(line, " ");
	int size = stoi(first_line[1]) + 1;
	bool* vars = new bool[size];

	for(int i = 0; i < size; i++){
		vars[i] = 0;
	}

	list<clause>* clauses = new list<clause>;
	list<list<int>>* negations = new list<list<int>>;
	while(getline(f, line)){
		size_t arrow = line.find("=>");
		if(arrow != string::npos){
			// Prase implication
			vector<string> split_line = split(line, "=>");
			string left = split_line[0];
			string right = split_line[1];

			// Parse left side of implication
			list<int>* left_list = new list<int>;
			vector<string> left_nums = split(left, "^");
			for(auto itr = left_nums.begin(); itr != left_nums.end(); itr++){
				string s_num = *itr;
				int i;
				for(i = 0; i < s_num.length(); i++){
					if(isdigit(s_num[i]))
						break;
				}
				s_num.erase(0, i);
				if(s_num.length()){
					left_list->push_back(stoi(s_num));
				}	
			}

			// Parse right side of implication
			int i;
			for(i = 0; i < right.length(); i++){
				if(isdigit(right[i]))
					break;
			}
			right.erase(0, i);
			clause clauz;
			if(right.length()){
				clauz.right = stoi(right);
				clauz.left = left_list;
			}
			clauses->push_back(clauz);
		} else {
			// Parse negation
			vector<string> str_nums = split(line, "v");
			list<int> negs;
			for(auto itr = str_nums.begin(); itr != str_nums.end(); itr++){
				string str_num = *itr;
				int i;
				for(i = 0; i < str_num.length(); i++){
					if(isdigit(str_num[i]))
						break;
				}
				str_num.erase(0, i);
				negs.push_back(stoi(str_num));
			}
			negations->push_back(negs);
		}
	}

	horn_formula* to_return = new horn_formula;
	to_return->size = size;
	to_return->variables = vars;
	to_return->implications = clauses;
	to_return->negations = negations;

	return to_return;
}

// Set insert and erase is constant in average case, linear in worst case
void horn_satisfaction(horn_formula* formula){
	list<clause>* clauses = formula->implications;
	bool* literal = formula->variables;
	unordered_set<int> working_set;
	
	// Add all singletons (have to be true) to working set
	for(auto itr = clauses->begin(); itr != clauses->end(); itr++){
		clause clauz = *itr;
		int single = clauz.is_singleton();
		if(single != -1){
			working_set.insert(single);
		}
	}

	// Every number is popped from the working set 
	// This number needs to be true, so its value is set to true
	// This while loop runs for a max k times where k is number of literals
	while(!working_set.empty()){
		auto start = working_set.begin();
		int v = *start;
		working_set.erase(start);
		// If a literal is true, it has already been removed from all clauses
		if(literal[v]){
			continue;
		}
		literal[v] = 1; 

		// Each time a literal is made true, it is removed from 
		// all the left sides' of the implication clauses
		// Since I am using a doubly-linked list, the removal itself is O(1)
		// This runs over all clauses at max k times. So it is O(kn), which
		// is still linear (just like DFS or BFS in a the case of a graph)
		for(auto itr = clauses->begin(); itr != clauses->end(); itr++){
			clause clauz = *itr;
			if(clauz.remove(v)){
				int single = clauz.is_singleton();
				if(single != -1){
					working_set.insert(single);
				}
			}
		}
	}
}

bool check_negation(horn_formula* formula){
	list<list<int>>* negations = formula->negations;
	bool* literals = formula->variables;
	
	for(auto i = negations->begin(); i != negations->end(); i++){
		bool check = false;

		list<int> negation = *i;
		for(auto j = negation.begin(); j != negation.end(); j++){
			int num = *j;
			if(literals[num] == false){
				check = true;
			}
		}
		
		if(check == false){
			return check;
		}
	}

	return true;
}

int main(){
	string filename;
	cout << "Input filename: ";
	cin >> filename;
	// filename = "q6  test case 3.txt";
	cout << endl;
	horn_formula* formula = parse(filename);
	horn_satisfaction(formula);
	// cout << endl;
	// formula->print();

	if(check_negation(formula)){
		cout << "Satisfiable! This is the assignment: " << endl;
		formula->print_literals();
	} else {
		cout << "Not Satisfiable" << endl;
	}
	// string s = "z10";
	// cout << stoi(s) << endl;

	return 0;
}