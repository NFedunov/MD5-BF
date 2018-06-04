#pragma once
#include <vector>
#include "struct.h"

struct maskElements
{
	char star[2]	= { 33, 126 };
	char d[2]		= { 48, 57 };
	char a[2]		= { 97, 122 };
	char A[2]		= { 65, 90 };
	char dot[2]		= { 65, 122 };
};

class WordGenerator
{
	size_t maxResultSize;
	maskElements mEl;
	std::string mask;
	std::string curWord;
	std::vector<password_s> result;
	std::string startWord;
	std::string lastWord;

	void genStartWord();
	void genNextWord(unsigned int i);
	char* handleMaskElement(char c);
	unsigned int handleConsts(unsigned int);
	bool done;
public:
	size_t wordSize();
	bool isOver();
	void clearResult();
	std::vector<password_s> getResult();
	size_t resultSize();
	void generateWords();
	WordGenerator(const char*, size_t);
};

