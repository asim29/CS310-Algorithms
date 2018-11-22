#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>
#include <fstream>

using namespace std;
/******************************* ALGORITHM DESCRIPTION *******************************
There is only one way this can be done in O(n) time complexity. This is possible by 
dividing the subproblem as follows:
Each node asks its left child what it's best Union of ancestries is, and also asks
for the two leaf nodes and the respective ancestries that allowed this to happen. It 
then asks for the same thing from it's right child. Now from these two results it has
6 options to return to its parent
1. The Left_Union is the max, so just add itself to it (and both leaf-ancestries) and return
2. The right_union is max, so add itself to it and both the leaf-ancestries
3. The Left Max 1 U Right Max 1 is best. Add itself to it and return
4. Left Max 1 U Right Max 2 is best
5. Left Max 2 U Right Max 1 is best
6. Left Max 2 U Right Max 2 is best.
It will simply select the best, add itself to all three ancestries (max1, max2 and Union)
and return that ancestry. 
The particular algorithm starts at the root, goes till the level right before the leaf
nodes - asks them to calculate the ancestry for themself (there is only one ancestry 
possible at that level), and then the cascading returns automatically give the answer.
It's basically one traversal forward and one backward. Hence it is in O(n)
*************************************************************************************/
struct leaf{
	int val;
	int index;
	int ancestry_sum;
	vector<int> an_nodes;
};

struct combined{
	leaf max1;
	leaf max2;
	int combined_sum;
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

int make_tree(int* &tree){
	bool random = false;
	int n;
	if(random){
		int h = 4;
		n = (h<<2)-1;
		cout << n << endl;
		tree = new int[n];		
		for(int i = 0; i < n; i++){
			int num = rand()%100 + 1;
			tree[i] = num;
		}
	} else {
		string filename = "q4_test_1.txt";
		string line;
		ifstream f(filename);
		getline(f, line);
		vector<string>  split_line = split(line, ", ");
		n = split_line.size();
		tree = new int[n];
		for(int i = 0; i < n; i++){
			tree[i] = stoi(split_line[i]);	
		}

	}
	// cout << "Returning " << n << endl;
	return n;
}

int l_child(int index){
	return 2*index+1;
}
int r_child(int index){
	return 2*index+2;
}

combined* best_combo(combined* left_comb, combined* right_comb){
	leaf lmax1 = left_comb->max1;
	leaf lmax2 = left_comb->max2;
	leaf rmax1 = right_comb->max1;
	leaf rmax2 = right_comb->max2;

	int l1r1 = lmax1.ancestry_sum + rmax1.ancestry_sum;
	int l1r2 = lmax1.ancestry_sum + rmax2.ancestry_sum;
	int l2r1 = lmax2.ancestry_sum + rmax1.ancestry_sum;
	int l2r2 = lmax2.ancestry_sum + rmax2.ancestry_sum;
	int l_comb = left_comb->combined_sum;
	int r_comb = right_comb->combined_sum;

	combined* best = new combined;

	if(l1r1 >= l1r2 && l1r1 >= l2r1 && l1r1 >= l2r2 && l1r1 >= l_comb && l1r1 >= r_comb){
		best->max1 = lmax1;
		best->max2 = rmax1;
		best->combined_sum = l1r1;	
	} else if(l1r2 >= l1r1 && l1r2 >= l2r1 && l1r2 >= l2r2 && l1r2 >= l_comb && l1r2 >= r_comb){
		best->max1 = lmax1;
		best->max2 = rmax2;
		best->combined_sum = l1r2;
	} else if(l2r1 >= l1r1 && l2r1 >= l1r2 && l2r1 >= l2r2 && l2r1 >= l_comb && l2r1 >= r_comb){
		best->max1 = lmax2;
		best->max2 = rmax1;
		best->combined_sum = l2r1;
	} else if(l2r2 >= l1r1 && l2r2 >= l1r2 && l2r2 >= l2r1 && l2r2 >= l_comb && l2r2 >= r_comb){
		best->max1 = lmax2;
		best->max2 = rmax2;
		best->combined_sum = l2r2;
	} else if(l_comb >= l1r1 && l_comb >= l1r2 && l_comb >= l2r1 && l_comb >= l2r2 && l_comb >= r_comb){
		best = left_comb;
	} else if(r_comb >= l1r1 && r_comb >= l1r2 && r_comb >= l2r1 && r_comb >= l2r2 && r_comb >= l_comb){
		best = right_comb;
	}

	return best;
}

combined* find_combined(int* tree, int i, int h, int level){
	int node = tree[i];
	if(level == h-1){
		int lchild = tree[l_child(i)];
		int rchild = tree[r_child(i)];
		leaf from_left;
		leaf from_right;

		from_left.val = lchild;
		from_left.index = l_child(i)+1;
		from_left.ancestry_sum = lchild + node;
		from_left.an_nodes.push_back(lchild);
		from_left.an_nodes.push_back(node);

		from_right.val = rchild;
		from_right.index = r_child(i)+1;
		from_right.ancestry_sum = rchild + node;
		from_right.an_nodes.push_back(rchild);
		from_right.an_nodes.push_back(node);

		combined* comb_anc = new combined;
		comb_anc->max1 = from_left;
		comb_anc->max2 = from_right;
		comb_anc->combined_sum = lchild + rchild + node;

		return comb_anc;
	}

	combined* left_comb = find_combined(tree, l_child(i), h, level+1);
	combined* right_comb = find_combined(tree, r_child(i), h, level+1);
	
	combined* to_ret = best_combo(left_comb, right_comb);
	to_ret->max1.ancestry_sum += node;
	to_ret->max1.an_nodes.push_back(node);
	to_ret->max2.ancestry_sum += node;
	to_ret->max2.an_nodes.push_back(node);

	to_ret->combined_sum += node;

	return to_ret;

}

void print_tree(int* tree, int n){
	for(int i = 0; i < n; i++){
		cout << tree[i] << ", ";
	}
	cout << endl;
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	int* tree;
	int n = make_tree(tree);
	// print_tree(tree, n);
	combined* ans = find_combined(tree, 0, 4, 1);

	int h = ((n+1)>>2) - 1;
	// cout << h << endl;
	int leaf_start = pow(2, h) - 1;
	leaf max_i = ans->max1;
	leaf max_j = ans->max2;
	int i = max_i.index - leaf_start;
	int j = max_j.index - leaf_start;
	cout << "xi = x" << i << endl;	
	cout << "xj = x" << j << endl;	
	cout << "Value of nodes in Ancestry (x" << i << ") = {";
	for(auto itr = max_i.an_nodes.begin(); itr != max_i.an_nodes.end(); itr++){
		cout << *itr << ", ";
	}
	cout << "}" << endl;
	cout << "Value of nodes in Ancestry (x" << j << ") = {";
	for(auto itr = max_j.an_nodes.begin(); itr != max_j.an_nodes.end(); itr++){
		cout << *itr << ", ";
	}
	cout << "}" << endl;

	cout << "Value of Max Ancesry (x" << i << ", x" << j << ") = " << ans->combined_sum << endl;
	return 0;
}
