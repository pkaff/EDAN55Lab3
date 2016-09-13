// IndependentSet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<vector>
#include<list>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<iostream>

using namespace std;

vector<vector<size_t> > graph(string filename) {
	ifstream ifs(filename);

	string line = "";
	size_t nV;
	ifs >> nV;
	getline(ifs, line);
	vector<vector<size_t> > G(nV);
	size_t row = 0;
	while (getline(ifs, line)) {
		istringstream iss(line);
		size_t v;
		for (size_t col = 0; col < nV; ++col) {
			iss >> v;
			if (v != 0) {
				G[row].push_back(col);
			}
		}
		++row;
	}
	return G;
}

class algR0 {
public:
	size_t iterations;

	algR0(vector<vector<size_t> > G) : graph(G), iterations(0) {

	}

	//main algorithm
	size_t R0(list<size_t> V) {
		++iterations;
		//case 1.
		if (V.size() == 0) {
			return 0;
		}

		//case 2, keep track of vertex with maximum degree for possible case 3.
		size_t maxDeg = 0;
		size_t u;

		for (auto i = V.begin(); i != V.end(); ++i) {
			size_t deg = graph[*i].size();
			if (deg == 0) {
				V.erase(i);
				return 1 + R0(V);
			}
			if (deg > maxDeg) {
				maxDeg = deg;
				u = *i;
			}
		}

		//case 3.
		V.remove(u);
		list<size_t> V2(V);
		//Think of a better way to do this
		for (const size_t& v : graph[u]) {
			V2.remove(v);
		}
		return max(1 + R0(V2), R0(V));
	}

private:
	vector<vector<size_t> > graph;
};



int main()
{
	string filename = "g30";
	ofstream ofs("C:\\Users\\biu\\Desktop\\lab3data\\output\\" + filename + ".out");
	auto G = graph("C:\\Users\\biu\\Desktop\\lab3data\\" + filename + ".in");
	algR0 r(G);
	list<size_t> V;

	for (size_t i = 0; i < G.size(); i++)
	{
		V.push_back(i);
	}
	size_t maxIS = r.R0(V);
	cout << maxIS << r.iterations << endl;
	ofs << "maximum independent set: " << maxIS << ", number of iterations: " << r.iterations << endl;
	return 0;
}

