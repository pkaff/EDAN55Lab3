// IndependentSet.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

class myVec {
private:
  vector<pair<size_t, bool> > vec;
public:
  size_t size;

  myVec() : size(0) {

  }
  myVec(myVec& other) {
    vec = other.vec;
    size = other.size;

  }

  pair<size_t, bool> get_pair(size_t i) {
    return vec[i];
  }

  void set(size_t i, bool b) {
    if (vec[i].second != b) {
      if (b) {
	++size;
      }
      else {
	--size;
      }
      vec[i].second = b;
    }
  }

  void push_back(pair<size_t, bool> p) {
    if (p.second) {
      ++size;
    }
    vec.push_back(p);
  }

  size_t max_size() {
    return vec.size();
  }

  bool peek(size_t i) {
    return vec[i].second;
  }

  size_t get(size_t i) {
    return vec[i].first;
  }
};

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

class algR {
public:
  size_t iterations;

  algR(vector<vector<size_t> > G) : graph(G), iterations(0) { 
		
  }

  //main algorithm
  size_t R0(myVec& V) {
    ++iterations;
    //case 1.
    if (V.size == 0) {
      return 0;
    }

    //case 2, keep track of vertex with maximum degree for possible case 3.
    size_t maxDeg = 0;
    size_t u;

    for (size_t i = 0; i < V.max_size(); ++i) {
      if (V.peek(i)) {
	size_t deg = 0;
	for (const size_t& n : graph[V.get(i)]) {
	  if (V.peek(n)) {
	    ++deg;
	  }
	}
	if (deg == 0) {
	  V.set(i, false);
	  return 1 + R0(V);
	}
	if (deg > maxDeg) {
	  maxDeg = deg;
	  u = V.get(i);
	}
      }
    }

    //case 3.
    V.set(u, false);
    myVec V2(V);
    for (const size_t& v : graph[u]) {
      V2.set(v, false);
    }
    return max(1 + R0(V2), R0(V));
  }

  //main algorithm
  size_t R1(myVec& V) {
    ++iterations;
    //case 1.
    if (V.size == 0) {
      return 0;
    }

    //case 2, keep track of vertex with maximum degree for possible case 3.
    size_t maxDeg = 0;
    size_t u;

    for (size_t i = 0; i < V.max_size(); ++i) {
      if (V.peek(i)) {
	size_t deg = 0;
	for (const size_t& n : graph[V.get(i)]) {
	  if (V.peek(n)) {
	    ++deg;
	  }
	}
	if (deg == 0) {
	  V.set(i, false);
	  return 1 + R1(V);
	}
	if (deg == 1) {
	  V.set(V.get(i), false);
	  for (const size_t& v : graph[V.get(i)]) {
	    V.set(v, false);
	  }
	  return 1 + R1(V);
	}
	if (deg > maxDeg) {
	  maxDeg = deg;
	  u = V.get(i);
	}
      }
    }

    //case 3.
    V.set(u, false);
    myVec V2(V);
    for (const size_t& v : graph[u]) {
      V2.set(v, false);
    }
    return max(1 + R1(V2), R1(V));
  }

    
		
private:
  vector<vector<size_t> > graph;
};



int main()
{
  //run r0
  for (size_t index = 30; index < 70; index += 10) {
    string filename = "g" + to_string(index) + ".txt";
    ofstream ofs("/h/d9/q/tpi11dod/Documents/Avancerade Algoritmer/EDAN55Lab3/IO_Data/output/r0/" + filename);
    auto G = graph("/h/d9/q/tpi11dod/Documents/Avancerade Algoritmer/EDAN55Lab3/IO_Data/input/" + filename);
    algR r(G);
    myVec V;

    for (size_t i = 0; i < G.size(); i++)
      {
	V.push_back(make_pair(i, true));
      }
    size_t maxIS = r.R0(V);
    cout << maxIS << " " << r.iterations << endl;
    ofs << "maximum independent set: " << maxIS << ", number of iterations: " << r.iterations << endl;
    ofs.close();
  }
  //run r1
  for (size_t index = 30; index < 110; index += 10) {
    string filename = "g" + to_string(index) + ".txt";
    ofstream ofs("/h/d9/q/tpi11dod/Documents/Avancerade Algoritmer/EDAN55Lab3/IO_Data/output/r1/" + filename);
    auto G = graph("/h/d9/q/tpi11dod/Documents/Avancerade Algoritmer/EDAN55Lab3/IO_Data/input/" + filename);
    algR r(G);
    myVec V;

    for (size_t i = 0; i < G.size(); i++)
      {
	V.push_back(make_pair(i, true));
      }
    size_t maxIS = r.R1(V);
    cout << maxIS << " " << r.iterations << endl;
    ofs << "maximum independent set: " << maxIS << ", number of iterations: " << r.iterations << endl;
    ofs.close();
  }
  return 0;
}

