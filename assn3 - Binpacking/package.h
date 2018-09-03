#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include "bin.h"//

class Package {
private:
	string infile;
	int bin_capa;
	int al_operation;
	string outfile;

	int bin_num;
	string al_type;
	double cal_time;
	clock_t tic;

	priority_queue<Pack, vector<Pack>, less<Pack> >ordered;//FOR DECREASING METHODS(FFD, BFD)
	queue<Pack> unordered;//FOR SIMPLE METHODS(FF, BF)
	vector<Pack> useless;//FOR OTHERS(KNAPSACK, FFDL2F)
	vector<Bin>bins;//CONTAINING BINS

public:
	Package(string in_name, string bin_capa, string al_operation, string out_name);
	void readfile();
	void solve();
	void writefile();
	void packing();
	void add_bins();//ADD ONE MORE BIN IN THE BINS LIST(VECTOR)
	
	void firstFit();
	void bestFit();
	void firstFitdecreasing();
	void bestFitdecreasing();
	void IterativeKnapsack();
	void Knapsack(int, int, vector<vector<int> >&, Bin&);
	void FFDL2();
	void L2F(Bin&, Pack);
};

#endif // !__PACKAGE_H__
