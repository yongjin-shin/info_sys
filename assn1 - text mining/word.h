#ifndef WORD_H
#define WORD_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
using namespace std;

/*
<Word Class>
	Saving word and Counting itself
*/
class Word {
private:
	string word;
	int cnt;

public:
	Word(string word) { this->word = word, cnt = 1; }
	void setWord(string word) { this->word = word; }
	void increCnt() { cnt++; }
	string getWord() { return word; }
	int getCnt() { return cnt; }
};

#endif
