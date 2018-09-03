#include "package.h"//

//INCLUDE FOLLOWING CLASSES:
// PACK CLASS: CONTAINS EACH PACK ID AND WEIGHT
// BIN CLASS: CONTAINS THE LIST OF PACKS AND ITS LIST'S INFO
// PACKAGE CLASS: CONTAINS THE LIST OF BINS, AND METHOD TO SOLVE PROBS
int main(int argc, char** argv){
  try{
    if(argc != 5)
      throw -1;

    Package pkg(argv[1], argv[2], argv[3], argv[4]);
    pkg.packing();//CONTAINS READ, SOLVE, WRITE METHOD
    cout << "Done: Everything!" << endl;

  }catch(int e){
    if(-1 == e){
		cout << "Please input correctly: like follows" << endl;
		cout << "Bin_packing.exe <item file name> <Bin size> <algorithm_type: 1 = FF, 2 = BF, 3 = FFD, 4 = BFD, 5 = Knapsack, 6 = FFD-L2F> <solution file name>" << endl;
      exit(-1);
    }
    if(-2 == e){
      cout << "Cannot open the infile!" << endl;
      exit(-2);
    }
    if(-3 == e){
      cout << "Wrong Input @ method choice" << endl;
      exit(-3);
    }
    if(-4 == e){
      cout << "Cannot open the outfile!" << endl;
      exit(-4);
    }
	if (-5 == e) {
		cout << "Full Bin!" << endl;
		exit(-5);
	}
  }
  return 0;
}
