#include "miner.h"

int main(int argc, char* argv[]) {
	try {
		clock_t	clockA;
		double elaspsedTime;
		clockA = clock();

		if (argc != 4) {
			cout << "Operation Error *.exe input_text.txt delimiter.txt output.txt" << endl;
			throw - 1;
		}

		Miner test;

		test.read_delimiter_file(argv[2]);					// read delimiter file
		test.read_text_file_and_parse(argv[1]);			// read text file
		test.calculateWordCount();									// calculate the number of total words
		test.bubbleSort();													// Sorting
		test.write_result(argv[3]);									// write the ouput

		cout << "Total Word # (Duplicated) = " << test.getTotalWordCount() << endl;
		cout << "Total Word # = " << test.getWordCount() << endl;
		elaspsedTime = (double)(double)(clock() - clockA) / CLOCKS_PER_SEC / 1000.0;
		cout << "Operating Duration = " <<fixed<< elaspsedTime << " milli seconds" << endl;

	}

	/*
	<exception cases>
	-1 is for wrong argc, argv
	-2 is for not being able to read text file
	-3 is for not being albe to read delimiter file
	-4 is for not being able to open output file
	*/
	catch (int exception) {
		if (-1 == exception)
			cout << "Input Error" << endl;
		else if (-2 == exception)
			cout << "First input file Error" << endl;
		else if (-3 == exception)
			cout << "Second input file Error" << endl;
		else if (-4 == exception)
			cout << "Out file Error" << endl;
	}
	catch (...) {
		cout << "Something Wrong!" << endl;
	}

  return 0;
}
