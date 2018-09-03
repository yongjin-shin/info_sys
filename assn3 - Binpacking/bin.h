#ifndef __BIN_H__
#define __BIN_H__

#include "pack.h"//

class Bin{
private:
	vector<Pack> bin;
	int capa; //THE CAPACITY OF A BIN. FIXED VALUE
	int rem; //THE REMAINING CAPACITY OF A BIN
	int min; //MINIMUM BOUNDARY FOR BESTFIT
	int max; //MAXIMUM BOUNDARY FOR BESTFIT

public:
	Bin(){}
	Bin(int capa) {
		this->capa = this->rem = capa;
		this->min = 5;
		this->max = 10;
	}

	int get_capa(){ return this->capa; }
	
	void update_rem(Pack new_pack) {
		this->rem -= new_pack.get_weight();
	}

	bool is_Full() {
		if (0 == rem)
			return true;
		return false;
	}

	//CHECK WHETHER THIS BIN CAN HAVE A NEW PACK
	bool is_Capable(Pack in_pack) {
		if (in_pack.get_weight() <= rem)
			return true;
		return false;
	}

	bool is_Capable(Pack one, Pack two) {
		if ((one.get_weight()+two.get_weight()) <= rem)
			return true;
		return false;
	}

	//CHECK THE BEST FIX CONDITIONS
	bool is_Best(Pack in_pack) {
		int diff = this->rem - in_pack.get_weight();
		if (diff < this->min || this->max < diff)
			return true;
		else if (is_empty())
			return true;
		return false;
	}

	bool is_empty() {
		if (this->capa == this->rem)
			return true;
		return false;
	}

	//ADD NEW PACK WITH UPDATING REMAINING CAPACITY
	void add(Pack new_pack) {
		if (this->is_Capable(new_pack)){
			this->bin.push_back(new_pack);
			this->update_rem(new_pack);
		}
		else
			throw - 5;
	}

	void add(Pack one, Pack two) {
		if (this->is_Capable(one, two)) {
			this->bin.push_back(one);
			this->bin.push_back(two);
			this->update_rem(one);
			this->update_rem(two);
		}
		else
			throw - 5;
	}


	void removeLast() {
		int size = this->bin.size();
		this->bin.erase(bin.begin()+(size - 1));
	}

	void write(ofstream& outfile) {
		int size = this->bin.size();
		outfile << size << ", " << this->capa - this->rem << endl;
		for (int i = 0; i < size; i++) {
			outfile << bin[i].get_id() << ", " << bin[i].get_weight() << endl;
		}
	}

	//CHECK WHETHER THIS BIN CAN HAVE TWO PACKS INSTEAD OF LAST PACK
	//WITH FFDL2F METHOD
	bool able_L2F(Pack new_pack, Pack smallest) {
		int old_rem = this->rem - new_pack.get_weight();
		if (old_rem == 0)
			return false;
		if (old_rem < smallest.get_weight())
			return true;
		return false;
	}
	

};

#endif // !__BIN_H__
