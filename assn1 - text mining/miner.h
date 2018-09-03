#ifndef MINER_H
#define MINER_H

#include "word.h"
#define MAX 1316134912;

/*
<Miner Class>
	Saving delimiter and word vector
	and the number of total word(duplicated)
*/
class Miner {
	vector<string> delimiter;
	vector<Word> txt;
	long m_total_word_count;

public:
	//read and write file
	void read_delimiter_file(const string filename);
	void read_text_file_and_parse(const string filename);
	void write_result(string filename);

	//sorting all words and finding same word
	int lookUp(vector<Word> txt, string tmp);
	void bubbleSort();
	void swap(Word& A, Word& B);

	//calculate the number of total words
	long getTotalWordCount() { return m_total_word_count; }
	long getWordCount() { return (long) txt.size(); }
	void calculateWordCount();

};


#endif
