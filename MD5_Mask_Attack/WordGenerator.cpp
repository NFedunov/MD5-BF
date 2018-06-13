#include "WordGenerator.h"
#include <iostream>

using namespace std;

bool WordGenerator::isOver()
{
	return done;
}

char* WordGenerator::handleMaskElement(const char c)
{
	switch (c)
	{
	case '*':
		return mEl.star;
	case 'd':
		return mEl.d;
	case 'a':
		return mEl.a;
	case '.':
		return mEl.dot;
	case 'A':
		return mEl.A;
	default:
		cout << "Error: unknown mask element '" << c << endl;
		return nullptr;
	}
}

unsigned int WordGenerator::handleConsts(unsigned int i)
{
	unsigned int j = i + 1;
	while (j < this->mask.size())
	{
		if (this->mask[j] == '\\')
		{
			if (this->mask[++j] == '\\')
			{
				this->mask.erase(j, 1);
				continue;
			}
			else if (this->mask[j] == ')')
			{
				return j;
			}
			else
			{
				cerr << "Error: wrong mask. -h for help." << endl;
				exit(-3);
			}
		}
		j++;

	}
	cerr << "Error: '\\)' not found!" << endl;
	exit(-3);
	return 0;
}

size_t WordGenerator::wordSize()
{
	return this->lastWord.size();
}

size_t WordGenerator::resultSize()
{
	return this->result.size();
}

void WordGenerator::genStartWord()
{
	unsigned int i = 0;
	
	while (i < mask.size())
	{
		if (mask[i] == '\\')
		{
			if (mask[i + 1] != '(')
			{
				cerr << "Error: wrong mask. -h for help" << endl;
				exit(-2);
			}
			unsigned int temp = this->handleConsts(i + 1);
			i += 2;
			startWord	+= mask.substr(i, temp - i - 1);
			curWord		+= mask.substr(i, temp - i - 1);
			lastWord	+= mask.substr(i, temp - i - 1);
			i = temp + 1;
			continue;
		}

		char* temp = &handleMaskElement(mask[i])[0];

		if (temp == nullptr)
		{
			exit(-1);
		}

		this->startWord += temp[0];
		this->curWord	+= temp[0];
		this->lastWord	+= temp[1];
		i++;
	}
}

WordGenerator::WordGenerator(const char* inputMask, size_t size)
{
	mask = string(inputMask);
	genStartWord();
	password_s newPass;
	memcpy_s(newPass.password, curWord.size() + 1, curWord.c_str(), curWord.size() + 1);

	result.push_back(newPass);
	maxResultSize = size;
	done = false;
}

vector<password_s> WordGenerator::getResult()
{
	return this->result;
}

void WordGenerator::clearResult()
{
	this->result.clear();
}

void WordGenerator::generateWords()
{
	while (curWord != lastWord && result.size() < maxResultSize)
	{
		genNextWord(0);
	}
	if (curWord == lastWord)
		done = true;
}

void WordGenerator::genNextWord(unsigned int i)
{
	if (curWord[i] == lastWord[i])
	{
		curWord[i] = startWord[i];
		genNextWord(i + 1);
	}
	else
	{
		curWord[i] += 1;
		password_s newPass;
		memcpy_s(newPass.password, curWord.size() + 1, curWord.c_str(), curWord.size() + 1);
	
		result.push_back(newPass);
		cout << "Generated word[" << result.size() << "]: " << result[result.size() - 1].password << endl;
	}
}