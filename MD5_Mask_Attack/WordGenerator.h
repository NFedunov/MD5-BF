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
	//max number of elements in array result
	size_t maxResultSize;
	maskElements mEl;
	std::string mask;
	//current processing word
	std::string curWord;
	std::vector<password_s> result;
	//first word created by mask
	std::string startWord;
	//last word
	std::string lastWord;

	//generates first word and last word
	void genStartWord();
	//generates next word after curWord
	void genNextWord(unsigned int i);
	//genNextWord uses this method for mask elements handling
	char* handleMaskElement(char c);
	//genNextWord uses this method for contant part handling
	unsigned int handleConsts(unsigned int);
	//all words was generated?
	bool done;
public:
	//returns size of generated words
	size_t wordSize();
	//returns all words generated or not
	bool isOver();
	//clears result vector
	void clearResult();
	//returns result vector
	std::vector<password_s> getResult();
	//returns result vector size
	size_t resultSize();
	//generates words while not done or 
	//size of result vector less than max size
	void generateWords();
	WordGenerator(const char*, size_t);
};

