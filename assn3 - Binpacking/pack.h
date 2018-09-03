#ifndef __PACK__H__
#define __PACK__H__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <ctime>
#include <functional>
#include <iomanip>
using namespace std;//

class Pack {
private:
	string id;
	int weight;
public:
	Pack(string id, string weight) {
		this->id = id;
		this->weight = stoi(weight);
	}

	int get_weight() const { return this->weight; }
	string get_id() const { return this->id; }
	friend bool operator<(const Pack& A, const Pack& B);
};

#endif // !__PACK__H__
