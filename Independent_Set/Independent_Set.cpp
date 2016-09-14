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
		size_t R0(vector<pair<size_t, bool> >& V) {
			++iterations;
			//case 1.
			bool end = true;
			for (const auto& it : V) {
				if (it.second == true) {
					end = false;
					break;
				}
			}
			if (end)
				return 0;

			//case 2, keep track of vertex with maximum degree for possible case 3.
			size_t maxDeg = 0;
			size_t u;

			for (auto& i : V) {
				if (i.second) {
					size_t deg = graph[i.first].size();
					if (deg == 0) {
						i.second = false;
						return 1 + R0(V);
					}
					if (deg > maxDeg) {
						maxDeg = deg;
						u = i.first;
					}
				}
			}

			//case 3.
			V[u].second = false;
			vector<pair<size_t, bool> > V2(V);
			for (const size_t& v : graph[u]) {
				V2[v].second = false;
			}
			return max(1 + R0(V2), R0(V));
		}
		
	private:
		vector<vector<size_t> > graph;
};



int main()
{
	for (size_t index = 30; index < 70; index += 10) {
		string filename = "g" + to_string(index);
		ofstream ofs("C:\\Users\\biu\\Desktop\\lab3data\\output\\" + filename + ".out");
		auto G = graph("C:\\Users\\biu\\Desktop\\lab3data\\" + filename + ".in");
		algR0 r(G);
		vector<pair<size_t, bool> > V;

		for (size_t i = 0; i < G.size(); i++)
		{
			V.push_back(make_pair(i, true));
		}
		size_t maxIS = r.R0(V);
		cout << maxIS << r.iterations << endl;
		ofs << "maximum independent set: " << maxIS << ", number of iterations: " << r.iterations << endl;
		ofs.close();
	}
	return 0;
}

