#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <list>
#include <cfloat> //DBL_MAX
#include <ctime>
#include <vector>
#include <cmath>
#include <ctime>
#include <map>
using namespace std;

//data sets
class Node{
private:
  string id;
  int x;
  int y;
  double dist;//distance update for dijkstra
  Node* next;
  double gs;//gscore for A*
  double fs;//fscore for A*
public:
	Node(string id = NULL, int x = 0, int y = 0, double dist = 0, Node* next = NULL, double gs = DBL_MAX, double fs = DBL_MAX) {
    this->id = id;
    this->x = x;
    this->y = y;
    this->next = next;
	this->dist = dist;
	this->gs = gs;
	this->fs = fs;
  }
  string get_id() const {return id;}
  double get_dist() const {return dist;}
  void set_dist(double dist){this->dist = dist;}
  int get_x() const {return x;}
  int get_y() const {return y;}
  void set_gs(double gs) { this->gs = gs; }
  void set_fs(double fs) { this->fs = fs; }
  double get_gs() { return this->gs; }
  double get_fs() { return this->fs; }
};

class Edge{
private:
  string here;
  Edge* there;//next node
public:
  Edge(){}
  Edge(string here, Edge* there = NULL){
    this->here = here;
    this->there = there;
  }
  string get_here() const {return this->here;}
  Edge* get_there() const {return this->there;}
  void set_there(Edge* there){this->there = there;}
  void set_init(string here, Edge* there){
    this->here = here;
    this->there = there;
  }
  bool has_next(){
    if(this->there != NULL)
      return true;
    return false;
  }
};

typedef struct{
  string from;
  string there;
  double dist;
} Pair;

//general used
void make_nodes(vector<Node>& nodes, const string av1){
  ifstream node_file;
  node_file.open(av1);
  if(!node_file.is_open())
    throw -1;

  string _id, x, y;
  int  _x, _y;
  getline(node_file, _id);
  while(!node_file.eof()){
    getline(node_file, _id, '\t');
    getline(node_file, x, '\t');
    getline(node_file, y);
    if(_id!= ""){
      _x = stoi(x);
      _y = stoi(y);
      Node tmp(_id, _x, _y);
      nodes.push_back(tmp);
    }
  }
}

void make_edges(vector<Edge>& edges, const vector<Node> nodes, const string av2){
  ifstream edge_file;
  edge_file.open(av2);
  if(!edge_file.is_open())
    throw -2;

  //from nodes data, firstly make default edge data
  string _id, n1, n2;
  int n_size = nodes.size();
  for(int i = 0; i<n_size; i++){
    string str = nodes[i].get_id();
    Edge tmp(str);
    edges.push_back(tmp);
  }

  //by reading file, we can update edge data
  //with look up header string
  getline(edge_file, _id);
  while(!edge_file.eof()){
    getline(edge_file, _id, '\t');
    getline(edge_file, n1, '\t');
    getline(edge_file, n2);
    for(int i = 0; i<n_size; ++i){
      if(edges[i].get_here() == n1){
        Edge* tmp = new Edge;
        tmp->set_init(n2, edges[i].get_there());
        edges[i].set_there(tmp);
      }
    }
    for(int i = 0; i<n_size; ++i){
      if(edges[i].get_here() == n2){
        Edge* tmp = new Edge;
        tmp->set_init(n1, edges[i].get_there());
        edges[i].set_there(tmp);
      }
    }
  }
}

int find_idx(string const target, const vector<Node> nodes){//search index with using string data from nodes
  int idx=-1;
  for (idx = 0; idx < nodes.size(); ++idx) {
	  string tmp = nodes[idx].get_id();
	  if (tmp == target)
		  break;
  }

  //if minus return, then it will show that no such node in the file
  if(idx == nodes.size())
    return -1;

  return idx;
}

int minDist(const vector<double> dist, const vector<bool> checked){
//return minimum index from the distance vector
//only if it is not checked
  double min = DBL_MAX;
  int min_idx = -1;

  for(int v = 0; v<dist.size(); v++)
    if(!checked[v] && dist[v] <= min)
      min=dist[v], min_idx = v;

  return min_idx;
}

double simpleCalc(int x1, int y1, int x2, int y2){
  int x = x1-x2;
  int y = y1-y2;
  return sqrt(x*x+y*y);
}

int find_src(const string dest, vector<Pair> path) {
	//with using string data
	//find header index from path data
	int idx = -1;
	for (idx = 0; idx < path.size(); idx++) {
		if (dest == path[idx].there)
			break;
	}

	return idx;
};

void minus4error(string av3, string start, string end) {
	//if the return value from "find_idx" function
	//this function will be called
	ofstream outfile;
	outfile.open(av3);
	if (!outfile.is_open())
		throw - 6;

	outfile << "Error: start - " << start << " end - " << end << endl;
	outfile << "No such Node in node lists" << endl;

}

//for dijkstra algorithm
double calcDist(Edge A, Edge B, vector<Node> nodes){
  double dist = -1;
  string here = A.get_here();
  string there = B.get_here();

  int h = find_idx(here, nodes);
  if(h<0)
    throw -4;
  int t = find_idx(there, nodes);
  if(t<0)
    throw -4;

  int h_x = nodes[h].get_x();
  int h_y = nodes[h].get_y();
  int t_x = nodes[t].get_x();
  int t_y = nodes[t].get_y();
  dist = simpleCalc(h_x, h_y, t_x, t_y);

  return dist;
}

void dijkstra(int hd, vector<Node> nodes, vector<Edge> edges, vector<double>& dist, vector<bool> checked, vector<Pair>& path){
  string head = nodes[hd].get_id();
  int i = -1;
  for(i = 0; i<edges.size(); ++i)
    if(edges[i].get_here() == head)
      break;

  if(i == edges.size() && edges[i].get_here() != head)
    throw -5;

  //designate specific header
  Edge from = edges[i];
  Edge* update = from.get_there();
  double btw;
  
  //until reach the end list
  while(NULL != update){
    Edge there = *update;
    int there_idx = find_idx(there.get_here(), nodes);
	//if it is already in checked, then skip it
	if (checked[there_idx]) {
		update = there.get_there();
		continue;
	}

	//calculate header to neighbor
	//if header distance + difference distance is bigger than neigbour's distance
	//skip it, otherwise update as follow:
    btw = calcDist(from, there, nodes);
    if(dist[hd] + btw < dist[there_idx]){
		int i;
		for (i = 0; i < path.size(); ++i) {
			if (path[i].there == there.get_here() && path[i].dist == dist[there_idx]) {
				dist[there_idx] = dist[hd] + btw;
				path[i].from = from.get_here();
				path[i].there = there.get_here();
				path[i].dist = dist[there_idx];
				break;
			}
		}
		if (i == path.size()) {
			dist[there_idx] = dist[hd] + btw;
			Pair tmp;
			tmp.from = from.get_here();
			tmp.there = there.get_here();
			tmp.dist = dist[there_idx];
			path.push_back(tmp);
		}

    }

    update = there.get_there();
  }
}

void make_solution(const string av3, vector<Node> nodes, stack<string> fin, vector<double> dist, clock_t tic, string start, string end) {
//write output file
//with using final stack
	ofstream outfile;
	outfile.open(av3);
	if (!outfile.is_open())
		throw - 6;

	queue<Node> st;
	
	//if the last element is not "start" node, then there is no way to get the source.
	string _s = fin.top();
	if (_s != start) {
		outfile << "Error: start - " << start << " end - " << end << endl;
		outfile << "Cannot find any path from src to dest" << endl;
		return;
	}
	//get top node from final stack which will be the start node
	while (!fin.empty()) {
		string th = fin.top();
		int idx = find_idx(th, nodes);
		Node tmp(th, nodes[idx].get_x(), nodes[idx].get_y(), dist[idx]);
		st.push(tmp);
		fin.pop();
	}
		
	int idx = find_idx(end, nodes);
	outfile << "Distance: " << dist[idx] << '\t';

	clock_t toc = clock();
	double time = toc - tic / CLOCKS_PER_SEC;
	outfile << "Time: " << time << "ms" << '\t';

	outfile << "Start Node: " << start << '\t';
	outfile << "End Node: " << end << endl;
	outfile << "Node" << '\t' << "x_coordinate" << '\t' << "y_coordinate" << '\t' << "distance to next node" << endl;

	double prev;
	int cnt_ = 1;
	while (!st.empty()) {
		Node tmp = st.front();
		double test = tmp.get_dist();
		if (1 != cnt_)//distance between two nodes
			outfile << test - prev << endl;
		outfile << tmp.get_id() << '\t' << tmp.get_x() << '\t' << tmp.get_y() << '\t';
		st.pop();
		prev = tmp.get_dist();
		cnt_++;
	}

}

//for A*algorithm
int minInopen(vector<string> openSet, vector<Node> nodes) {
//find minimum index form openSet, which we will look up for
	int min_idx = -1;
	double min = DBL_MAX;
	int size = openSet.size();
	for (int i = 0; i < size; ++i) {
		int idx = find_idx(openSet[i], nodes);
		double tmp = nodes[idx].get_fs();
		if (tmp <= min)
			min = tmp, min_idx = i;
	}
	return min_idx;
}

double calcDist2(Node A, Node B) {
	double dist = -1;
	int a_x = A.get_x();
	int a_y = A.get_y();
	int b_x = B.get_x();
	int b_y = B.get_y();
	dist = simpleCalc(a_x, a_y, b_x, b_y);
	return dist;
};

bool In_closed(string target, vector<string> closedSet) {
//if whether target string is closed Set
//if it is in closed Set, then we don't need to look up that node
	int size = closedSet.size();
	for (int i = 0; i < size; ++i)
		if (target == closedSet[i])
			return true;

	return false;
};

bool In_open(string target, vector<string> openSet) {
//if it is in open set, we need to look it up
	int size = openSet.size();
	for (int i = 0; i < size; ++i)
		if (target == openSet[i])
			return true;

	return false;
}

void cantFindA(string av3, string start, string end) {
//if there is some error in finding path
//then write in output file
	ofstream outfile;
	outfile.open(av3);
	if (!outfile.is_open())
		throw - 6;

	outfile << "Error: start - " << start << " end - " << end << endl;
	outfile << "Cannot find any path from src to dest" << endl;

};

void FindA(const string av3, vector<Node> nodes, vector<Pair> cameFrom, string crnt, string src, clock_t tic) {
//if there is a path from source to destination
	ofstream outfile;
	outfile.open(av3);
	if (!outfile.is_open())
		throw - 6;

	int n_size = nodes.size();
	int crnt_idx = find_idx(crnt, nodes);
	int src_idx = find_idx(src, nodes);

	outfile << "Distance: " << nodes[crnt_idx].get_fs() << '\t';

	clock_t toc = clock();
	double time = toc - tic / CLOCKS_PER_SEC;
	outfile << "Time: " << time << "ms" << '\t';

	outfile << "Start Node: " << nodes[src_idx].get_id() << '\t';
	outfile << "End Node: " << nodes[crnt_idx].get_id() << endl;
	outfile << "Node" << '\t' << "x_coordinate" << '\t' << "y_coordinate" <<'\t' << "distance to next node" << endl;

	stack<Node> path;
	string dst = crnt;
	while (1) {
		int idx = find_src(dst, cameFrom);
		if(idx<0 || idx == nodes.size())
			break;
		else {
			int i_dst = find_idx(dst, nodes);
			Node tmp_dst = nodes[i_dst];
			path.push(tmp_dst);
			dst = cameFrom[idx].from;
		}

	}
	
	int prev_x, prev_y;
	int cnt_ = 1;
	while (!path.empty()) {
		Node crnt = path.top();
		if (1 != cnt_)
			outfile << simpleCalc(crnt.get_x(), crnt.get_y(), prev_x, prev_y) << endl;
		outfile << crnt.get_id() << '\t' << crnt.get_x() << '\t' << crnt.get_y() << '\t';
		path.pop();
		prev_x = crnt.get_x();
		prev_y = crnt.get_y();
		cnt_++;
	}
};

void A(const int n_size, vector<Node>& nodes, const string dest, const string src, vector<Edge> edges, string av3, clock_t tic) {
//real A* algorithm
	vector<string> closedSet;
	vector<string> openSet;
	openSet.push_back(src);
	
	vector<Pair> cameFrom;//path data
	for (int i = 0; i < n_size; i++) {
		string head = nodes[i].get_id();
		Pair tmp;
		tmp.there = head;
		cameFrom.push_back(tmp);
	}

	cout << "done2" << endl;

	int src_idx = find_idx(src, nodes);
	if (src_idx < 0) {
		minus4error(av3, src, dest);
		throw - 4;
	}
	int dest_idx = find_idx(dest, nodes);
	if (dest_idx < 0) {
		minus4error(av3, src, dest);
		throw - 4;
	}
	nodes[src_idx].set_gs(0);
	nodes[src_idx].set_fs(calcDist2(nodes[src_idx], nodes[dest_idx]));

	//until we will look up all the element in openSet
	while (!openSet.empty()) {
		//start with minimum f_score
		int ck = minInopen(openSet, nodes);
		string crnt = openSet[ck];
		if (crnt == dest)
			return FindA(av3, nodes, cameFrom, crnt, src, tic);
		
		//fetch from openset
		//and put it in the closed Set not to touch after iteration
		//same with checked in dijkstra
		openSet.erase(openSet.begin() + ck);
		closedSet.push_back(crnt);
		
		int crnt_idx = find_idx(crnt, nodes);
		Edge from = edges[crnt_idx];
		Edge* update = from.get_there();

		//look up all the neighbours of current nodes
		while (NULL != update) {
			Edge there = *update;
			string neighbor = there.get_here();
			if (In_closed(neighbor, closedSet)) {
				update = there.get_there();
				continue;
			}
			if (!In_open(neighbor, openSet))
				openSet.push_back(neighbor);

			//compare between current distance + distance between neighbour to header
			//the minimum will be the only path
			int neighbor_idx = find_idx(neighbor, nodes);
			double tent_gs = nodes[crnt_idx].get_gs() + calcDist2(nodes[crnt_idx], nodes[neighbor_idx]);
			if (tent_gs >= nodes[neighbor_idx].get_gs()) {
				update = there.get_there();
				continue;
			}

			//update camefrom(path)
			cameFrom[neighbor_idx].from = nodes[crnt_idx].get_id();
			nodes[neighbor_idx].set_gs(tent_gs);
			double btw = calcDist2(nodes[neighbor_idx], nodes[dest_idx]);
			nodes[neighbor_idx].set_fs(nodes[neighbor_idx].get_gs() + btw);

			update = there.get_there();
		}
	}

	cantFindA(av3, src, dest);
	throw - 10;
};


int main(int argc, char** argv){
  try{
	  if (argc != 7)
		  throw - 7;
	  //exe, node, edge, start, end, option, outfile
    string av1 = argv[1];//node
    string av2 = argv[2];//edge
    string av3 = argv[6];//out
    string src = argv[3];//start
    string dest = argv[4];//end
	string end = dest;
    int n_size = 0;
	int op = stoi(argv[5]);//option

	clock_t tic = clock();

    vector<Node> nodes;
    vector<Edge> edges;
    make_nodes(nodes, av1);
    make_edges(edges, nodes, av2);
    n_size = nodes.size();
	cout << "1done" << endl;
	
	int src_i_ = find_idx(src, nodes);
	if (src_i_< 0) {
		minus4error(av3, src, dest);
		throw - 4;
	}

	int dest_i_ = find_idx(dest, nodes);
	if (dest_i_< 0) {
		minus4error(av3, src, dest);
		throw - 4;
	}

	switch (op) {
	case 1: {
		vector<double> dist;
		vector<bool> checked;
		vector<Pair> path;
		for (int i = 0; i<n_size; ++i) {
			dist.push_back(DBL_MAX);
			checked.push_back(false);
		}
		cout << "2done" << endl;

		int s = find_idx(src, nodes);
		if (s < 0) {
			minus4error(av3, src, dest);
			throw - 4;
		}
		dist[s] = 0;
		cout << "2.5done" << endl;

		for (int cnt = 0; cnt <n_size; cnt++) {
			int u = minDist(dist, checked);
			checked[u] = true;
			dijkstra(u, nodes, edges, dist, checked, path);
		}
		cout << "3done" << endl;

		stack<string> fin;
		while (1) {
			fin.push(dest);
			int idx = find_src(dest, path);
			if (idx < 0 || idx == path.size())
				break;
			else
				dest = path[idx].from;
		}
		cout << "4done" << endl;

		make_solution(av3, nodes, fin, dist, tic, src, end);
		cout << "5done" << endl;

		break;
	}

	case 2: {
		A(n_size, nodes, dest, src, edges, av3, tic);
		break;
	}
	default:
		throw - 8;
	}

  }catch(int e){
	  if (-1 == e)
		  cout << "Cannot open node.txt" << endl;
	  else if (-2 == e)
		  cout << "Cannot open edge.txt" << endl;
	  else if (-3 == e)
		  cout << "Cannot open path.txt" << endl;
	  else if (-4 == e)
		  cout << "No such node in file" << endl;
	  else if (-5 == e)
		  cout << "Seg falut?!" << endl;
	  else if (-6 == e)
		  cout << "Cannot open out file" << endl;
	  else if (-7 == e)
		  cout << "Argc error: Please input as follows: " << endl << "my_shortest_path.exe <node file name> <arc file name> <start node ID> <end node ID> <algorithm_type: 1 = Dijkstra, 2 = A*> <solution file name>"<< endl;
	  else if (-8 == e)
		  cout << "Wrong operation" << endl;
	  else if (-10 == e)
		  cout << "Cannot find" << endl;
  }
}
