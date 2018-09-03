#include "miner.h"
void Miner::read_delimiter_file(const string filename){
	ifstream deli;
  string line;
  deli.open(filename);
  if (!deli.is_open()) {
    cout << "Cannot find the file" << endl;
    throw -2;
  }

	//if line is not "\n"
	//then, save it into the vector
  while (!deli.eof()) {
    getline(deli, line);
    if (line != "") {
      delimiter.push_back(line);
    }
  }

  deli.close();
}


void Miner::read_text_file_and_parse(const string filename){
  ifstream infile;
  string line;
  infile.open(filename);
  if (!infile.is_open()) {
    cout << "Cannot find the file" << endl;
    throw -2;
  }

	//First, parsing
	//Second, check whether the parsed word is delimiter or not
	//Third, check wheter the parsed word has alread been in the vector
	//Fourth, if it is in the vecotr, just count up.
	//Otherwise, update the vector
  while (!infile.eof()) {
		getline(infile, line);
		int end = 0U;

		while (end != string::npos) {
			int i;
		  long min = MAX;
			int min_i = 0;

			//First, Parsing with finding the position which has the minimum distance
			for (i = 0U; i < delimiter.size(); i++) {
				if (-1 != line.find(delimiter[i])) {
					end = line.find(delimiter[i]);
					if (end < min) {
						min = end;
						min_i = i;
					}
				}
			}

			//Second, if the position is 0, then it should be delimiter
			if (0 == min) {
				end = line.find(delimiter[min_i]);
				line.erase(0, min + delimiter[min_i].length());
				continue;
			}
			else {
				string tmp = line.substr(0, min);

				//Or, if it is exactly same with delimiter, then delete it
				//without updating word vector!
				bool checked = false;
				for (int i = 0; i < delimiter.size(); i++) {
					if (tmp == delimiter[i] || tmp == "" || tmp == " ") {
						end = line.find(delimiter[min_i]);
						line.erase(0, min + delimiter[min_i].length());
						checked = true;
					}
				}

				if (checked)
					continue;

				//Third, find same value in the vector
				//if there is, then increase counting
				if (lookUp(txt, tmp))
					txt[lookUp(txt, tmp)].increCnt();
				else
					txt.push_back(tmp);

				//Finaly, Erase the parsed word from the original line.
				end = line.find(delimiter[min_i]);
				line.erase(0, min + delimiter[min_i].length());
			}
		}
	}

  infile.close();
}

//check two stings
//if they are different, then return 0
int Miner::lookUp(vector<Word> txt, string tmp) {
	for (int i = 0; i < txt.size(); i++)
		if (0 == tmp.compare(txt[i].getWord()))
			return i;
	return 0;
}

//Just simple bubble sorting
void Miner::bubbleSort(){
	for (int i = 0; i < txt.size(); i++) {
		for (int j = txt.size() - 1; j > i; j--) {
			if (txt[j - 1].getCnt() < txt[j].getCnt())
				swap(txt[j - 1], txt[j]);
		}
	}
}

void Miner::swap(Word& A, Word& B) {
	Word tmp = A;
	A = B;
	B = tmp;
}

//update each txt words' count
void Miner::calculateWordCount(){
  for(int i = 0; i<txt.size(); i++){
    m_total_word_count += txt[i].getCnt();
  }
}

void Miner::write_result(string filename) {
	ofstream outfile;
	outfile.open(filename);
	if (!outfile.is_open()) {
		cout << "Cannot open the file" << endl;
		throw - 4;
	}

	outfile << "Total Word # (Duplicated) = " << m_total_word_count << endl;
	outfile << "Total Word # = " << txt.size() << endl;
	outfile << "WORD  |  COUNT" << endl;
	for (int i = 0; i < txt.size(); i++)
		outfile << txt[i].getWord() << " " << txt[i].getCnt() << endl;

	outfile.close();
}
