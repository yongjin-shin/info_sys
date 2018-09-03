#include "package.h"//

Package::Package(string in_name, string bin_capa, string al_operation, string out_name) {
	this->infile = in_name;
	this->bin_capa = stoi(bin_capa);
	this->al_operation = stoi(al_operation);
	this->outfile = out_name;
	this->bin_num = 0;
	Bin start(this->bin_capa);
	this->bins.push_back(start);
}

void Package::readfile() {
	ifstream infile;
	infile.open(this->infile);
	if (!infile.is_open())
		throw - 2;

	string line;
	getline(infile, line);
	while (!infile.eof()) {
		string id, weight;
		getline(infile, id, '\t');
		getline(infile, weight);
		if (id != "" && weight != "") {
			Pack tmp(id, weight);
			ordered.push(tmp);
			unordered.push(tmp);
			useless.push_back(tmp);
		}
	}
	cout << "Done: Reading File!" << endl;
}

void Package::solve() {
	this->tic = clock();
	switch (this->al_operation) {
	case 1:
		this->al_type = "First Fit";
		firstFit();
		cout << "Done: Solved - " << this->al_type << endl;
		break;
	case 2:
		this->al_type = "Best Fit";
		bestFit();
		cout << "Done: Solved - " << this->al_type << endl;
		break;
	case 3:
		this->al_type = "First Fit Decreasing";
		firstFitdecreasing();
		cout << "Done: Solved - " << this->al_type << endl;
		break;
	case 4:
		this->al_type = "Best Fit Decreasing";
		bestFitdecreasing();
		cout << "Done: Solved - " << this->al_type << endl;
		break;
	case 5:
		this->al_type = "Iterative Knapsack Algorithm";
		IterativeKnapsack();
		cout << "Done: Solved - " << this->al_type << endl;
		break;
	case 6:
		this->al_type = "First Fit Decreasing, Last Two Fit";
		FFDL2();
		cout << "Done: Solved - " << this->al_type << endl;
		break;
	default:
		throw - 3;
	}
}

void Package::writefile() {
	ofstream outfile;
	outfile.open(this->outfile);
	if (!outfile.is_open())
		throw - 4;
	double time_tmp = (double)(clock() - this->tic);
	this->cal_time = time_tmp / CLOCKS_PER_SEC;
	this->cal_time *= 1000;


	outfile << "Input File: " << this->infile << " Bin Capacity: " << this->bin_capa << " algorithm type: " << this->al_type << " # of Bin: " << this->bin_num << " Time: " << this->cal_time << "ms" << endl;
	int i = 0;
	int size = this->bins.size();
	for (i = 0; i < size; i++) {
		outfile << "Bin #" << i+1 << ": ";
		bins[i].write(outfile);
	}

	cout << "Done: Writing File!" << endl;
}

void Package::packing() {
	this->readfile();
	this->solve();
	this->writefile();
}

bool operator<(const Pack& A, const Pack& B) {
	if (A.get_weight() < B.get_weight())
		return true;

	return false;
}

void Package::add_bins() {
	Bin tmp(this->bin_capa);
	this->bins.push_back(tmp);
}

void Package::firstFit() {
	while (!unordered.empty()) {
		Pack new_pack = unordered.front();
		int i = 0;
		int size = this->bins.size();
		for (i = 0; i < size; ++i) {
			if (bins[i].is_Capable(new_pack)) {
				bins[i].add(new_pack);
				break;
			}
		}

		//IF THERE IS NOT BIN TO HAVE THIS PACK
		//THEN, ADD ONE MORE BIN AND JUST PUSH THIS PACK INTO IT
		if (i >= size) {
			this->add_bins();
			this->bins.back().add(new_pack);
		}

		unordered.pop();
	}
	this->bin_num = this->bins.size();
}

void Package::bestFit() {
	while (!unordered.empty()) {
		Pack new_pack = unordered.front();
		int i = 0;
		int size = this->bins.size();
		for (i = 0; i < size; ++i) {
			//ANOTHER ADDITIONAL CHECK FOR THE BEST FIT!
			if (bins[i].is_Capable(new_pack) && bins[i].is_Best(new_pack)) {
				bins[i].add(new_pack);
				break;
			}
		}

		//IF THERE IS NOT BIN TO HAVE THIS PACK
		//THEN, ADD ONE MORE BIN AND JUST PUSH THIS PACK INTO IT
		if (i >= size) {
			this->add_bins();
			this->bins.back().add(new_pack);
		}

		unordered.pop();
	}

	this->bin_num = this->bins.size();
}

void Package::firstFitdecreasing() {
	//USING ORDERED LISTS
	//EXCEPT, SAME WITH FIRSTFIT

	while (!ordered.empty()) {
		Pack new_pack = ordered.top();
		int i = 0;
		int size = this->bins.size();
		for (i = 0; i < size; ++i) {
			if (bins[i].is_Capable(new_pack)) {
				bins[i].add(new_pack);
				break;
			}
		}

		if (i >= size) {
			this->add_bins();
			this->bins.back().add(new_pack);
		}

		ordered.pop();
	}

	this->bin_num = this->bins.size();
}

void Package::bestFitdecreasing() {
	//USING ORDERED LISTS
	//EXCEPT, SAME WITH BESTFIT

	while (!ordered.empty()) {
		Pack new_pack = ordered.top();
		int i = 0;
		int size = this->bins.size();
		for (i = 0; i < size; ++i) {
			if (bins[i].is_Capable(new_pack) && bins[i].is_Best(new_pack)) {
				bins[i].add(new_pack);
				break;
			}
		}

		if (i >= size) {
			this->add_bins();
			this->bins.back().add(new_pack);
		}

		ordered.pop();
	}

	this->bin_num = this->bins.size();
}

void Package::IterativeKnapsack() {
	const int c = bins.back().get_capa();//SAVE THE CAPACITY OF EACH BIN
	while (1) {
		//2D MATRIX F FOR SAVING KNAPSACK SOLVER
		int n = useless.size();
		vector<vector<int> > F;
		for (int i = 0; i < n + 1; i++) {
			vector<int> f;
			for (int j = 0; j < c + 1; j++)
				f.push_back(0);
			F.push_back(f);
		}

		Knapsack(c, n, F, bins.back());

		//IF VECTOR IS NOT EMPTY, THEN ADD NEW BIN
		//IF THE SIZE IS 1, THEN JUST PUSH THE LAST ONE AND BREAK IT!
		if (1 < useless.size())
			this->add_bins();
		else if (1 == useless.size()) {
			this->add_bins();
			this->bins.back().add(useless.back());
			useless.pop_back();
			break;
		}
		else
			break;
	}

	this->bin_num = this->bins.size();
}

void Package::Knapsack(int c, int n, vector<vector<int> >& F, Bin& bin) {
	Pack empty("empty", "0");
	vector<Pack> w = this->useless;
	w.insert(w.begin(), empty);

	//KNAPSACK SOLVER WITH USING 2D MATRIX F
	//FIND MAXIMUM VALUE; WHICH WILL HAVE MAXIMUM WEIGHTS IN TOTAL
	//USING WEIGHT INSTEAD OF P(N)
	int w_n = w[n].get_weight();
	for (int y = 0; y < w_n; y++)
		F[n][y] = 0;
	for (int y = w_n; y <= c; y++)
		F[n][y] = w[n].get_weight();

	for (int i = n - 1; i > 1; i--) {
		int w_i = w[i].get_weight();
		for (int y = 0; y < w_i; y++)
			F[i][y] = F[i + 1][y];
		for (int y = w_i; y <= c; y++)
			F[i][y] = max(F[i + 1][y], F[i + 1][y - w_i] + w[i].get_weight());
	}

	F[1][c] = F[2][c];
	if (c >= w[1].get_weight())
		F[1][c] = max(F[1][c], F[2][c - w[1].get_weight()] + w[1].get_weight());

	//KNAPSACK TRACEBACK PROCESS
	//WITH USING VECTOR:X WHICH CONTAINS PACKS' INFO TO BE SAVED
	vector<int> x;
	x.push_back(0);
	for (int i = 1; i < n; i++) {
		if (F[i][c] == F[i + 1][c])
			x.push_back(0);
		else {
			x.push_back(1);
			c -= w[i].get_weight();
		}
	}
	if (F[n][c])
		x.push_back(1);
	else
		x.push_back(0);

	//MARKED PACKS' TO BE SAVED AS "EMPTY"
	int size = x.size();
	for (int i = 0; i < size; i++) {
		if (x[i] == 1) {
			bin.add(useless[i-1]);
			*(useless.begin() + i - 1) = empty;
		}
	}

	//DELETE PACKS MARKED WITH "EMPTY"
	int i = 0;
	size = useless.size();
	while(i < size) {
		if (useless[i].get_id() == "empty") {
			useless.erase(useless.begin() + i);
			i = 0;
			size = useless.size();
			continue;
		}
		i++;
	}
}

void Package::FFDL2() {
	while (!useless.empty()) {
		//SORT FIRST, LIKE FFD
		sort(useless.begin(), useless.end());
		Pack new_pack = useless.back();
		int i = 0;
		int size = this->bins.size();
		for (i = 0; i < size; ++i) {
			//CHECK L2F
			//IF AVAILABLE THEN DO L2F
			if (bins[i].is_Capable(new_pack)) {
				if (useless.size() != 1 && bins[i].able_L2F(new_pack, useless.front())) {
					L2F(bins[i], new_pack);
				} else {
					bins[i].add(new_pack);
					useless.pop_back();
				}
				break;
			}
		}
		if (i >= size) {
			this->add_bins();
			this->bins.back().add(new_pack);
			useless.pop_back();
		}
	}
	this->bin_num = this->bins.size();
}

void Package::L2F(Bin& bin, Pack new_pack) {
	int size = this->useless.size();
	int i, j;
	
	//KEEP SAVING THE BIGGEST PAIR OF PACKS
	int big_i, big_j; big_i = big_j = 0;
	int big_weight = useless[big_i].get_weight() + useless[big_j].get_weight();
	
	//GEEDY SEARCH FOR THE BIGGEST PAIRS
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			if (i == j)
				continue;
			Pack one = useless[i];
			Pack two = useless[j];
			if(bin.is_Capable(one, two)) {
				int two_weight = one.get_weight() + two.get_weight();
				if (big_weight <= two_weight) {
					big_i = i;
					big_j = j;
				}
			}
		}
	}

	//IF THERE IS NO PROPER PAIR OF PACKS,
	//THEN JUST PUSH THE ORIGINAL ONE
	if (size <= i && size <= j && big_i == big_j) {
		bin.add(new_pack);
		useless.pop_back();
		return;
	}

	//IF THE PROPER PAIR IS FOUND,
	//DELETE THOSE PAIRS FROM THE LIST
	bin.add(useless[big_i], useless[big_j]);
	if (big_i > big_j) {
		useless.erase(useless.begin() + big_i);
		useless.erase(useless.begin() + big_j);
	} else {
		useless.erase(useless.begin() + big_j);
		useless.erase(useless.begin() + big_i);
	}
}
