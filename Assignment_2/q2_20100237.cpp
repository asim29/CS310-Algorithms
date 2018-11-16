#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

struct car_and_locs{
	int T;
	int R;
	int F;
	int N;

	int* stations;

	void print(){
		cout << "Capacity: " << T << " litres" << endl;
		cout << "Consumption: " << R << " litres per km" << endl;
		cout << "Fill Rate: " << F << " litres per minutes" << endl;
		
		cout << "The stations are: " << endl;
		int i = 0;
		cout << "Starting Point O: " << stations[i] << endl;
		for(i = 1; i < N-1; i++){
			cout << "L" << i << ": " << stations[i] << endl;
		}
		cout << "H" << ": " << stations[N-1] << endl;
	}
};

int getnum(string line){
	size_t pos = line.find(" ");
	line.erase(0, pos+1);
	return stoi(line);
}

car_and_locs* parse_text(string filename){
	string line;
	ifstream f(filename);

	getline(f, line);
	int T = getnum(line);
	getline(f, line);
	int R = getnum(line);
	getline(f, line);
	int F = getnum(line);
	getline(f, line);
	int N = getnum(line) + 2;

	int* stations = new int[N];
	stations[0] = 0;
	for(int i = 1; i < N; i++){
		getline(f, line);
		int dist = getnum(line);
		stations[i] = dist;
	}

	car_and_locs* ret = new car_and_locs;
	ret->T = T;
	ret->R = R;
	ret->F = F;
	ret->N = N;
	ret->stations = stations;
	return ret;
}

void find_min_time(car_and_locs* all, int cur_station,int cur_fuel,double &total_time){
	int T = all->T;
	int R = all->R;
	int F = all->F;
	int N = all->N;
	int* stations = all->stations;
	// Find farthest station it can go to with current fuel and go there
	for(int i = N-1; i > 0; i--){
		int fuel_req = R*(stations[i] - stations[cur_station]);
		if(fuel_req <= cur_fuel){
			cur_station = i;
			cur_fuel -= fuel_req;
			break;
		}
	}

	if(cur_station == N-1){
		// cout << "Reached!" << endl;
		return;
	}

	// Find next furthest it can go to, and fill accordingly
	for(int i = N-1; i > 0; i--){
		int fuel_req = R*(stations[i] - stations[cur_station]);
		if(fuel_req <= T){
			int to_fill = fuel_req - cur_fuel;
			double time_taken = ((double)to_fill/ (double)F);
			cout << "L:" << cur_station << " " << time_taken << endl;
			total_time += time_taken;
			cur_fuel += to_fill;
			break;
		}
	}

	find_min_time(all, cur_station, cur_fuel, total_time);
}

int main()
{	
	string filename;
	// cout << "Input filename: ";
	// getline(cin, filename);
	// cout << filename << endl;
	filename = "q2 test case 1.txt";
	// filename = "q2 test case 3.txt";
	car_and_locs* everything = parse_text(filename);
	// cout << "Here" << endl;
	// everything->print();
	double time = 0;
	find_min_time(everything, 0, everything->T, time);

	cout << "Total time: " << time << endl;
	return 0;
}