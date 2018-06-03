#pragma once
#include "WordGenerator.h"
#include <iostream>
#include <vector>
#include "struct.h"

using namespace std;

struct maskElements {
	char star[2] = { 33, 126 };
	char d[2] = { 48, 57 };
	char a[2] = { 97, 122 };
	char A[2] = { 65, 90 };
	char dot[2] = { 65, 122 };
};

class WordGenerator
{
	size_t maxResultSize;
	maskElements mEl;
	string mask;
	string curWord;
	vector<password_s> result;
	string startWord;
	string lastWord;

	void genStartWord();
	void genNextWord(unsigned int i);
	char* handleMaskElement(char c);
	unsigned int handleConsts(unsigned int);
	bool done;
public:
	size_t wordSize();
	bool isOver();
	void clearResult();
	vector<password_s> getResult();
	size_t resultSize();
	void generateWords();
	WordGenerator(const char*, size_t);
	~WordGenerator();
};

