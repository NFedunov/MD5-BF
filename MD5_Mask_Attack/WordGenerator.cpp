#include "WordGenerator.h"

bool WordGenerator::isOver()
{
	return this->done;
}

char* WordGenerator::handleMaskElement(char c)
{
	switch (c)
	{
	case '*':
		return this->mEl.star;
	case 'd':
		return this->mEl.d;
	case 'a':
		return this->mEl.a;
	case '.':
		return this->mEl.dot;
	case 'A':
		return this->mEl.A;
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
	while (i < this->mask.size())
	{
		if (this->mask[i] == '\\')
		{
			if (this->mask[i + 1] != '(')
			{
				cerr << "Error: wrong mask. -h for help" << endl;
				exit(-2);
			}
			unsigned int temp = this->handleConsts(i + 1);
			i += 2;
			this->startWord += this->mask.substr(i, temp - i - 1);
			this->curWord += this->mask.substr(i, temp - i - 1);
			this->lastWord += this->mask.substr(i, temp - i - 1);
			i = temp + 1;
			continue;
		}
		char *temp = &this->handleMaskElement(this->mask[i])[0];
		if (temp == nullptr)
		{
			exit(-1);
		}
		this->startWord += temp[0];
		this->curWord += temp[0];
		this->lastWord += temp[1];
		i++;
	}
}

WordGenerator::WordGenerator(const char* mask, size_t size)
{
	this->mask = string(mask);
	this->curWord = "";
	this->genStartWord();
	password_s newPass;
	memcpy_s(newPass.password, this->curWord.size() + 1, this->curWord.c_str(), this->curWord.size() + 1);;
	this->result.push_back(newPass);
	this->maxResultSize = size;
	this->done = false;
}


WordGenerator::~WordGenerator()
{
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
	while (this->curWord != this->lastWord && this->result.size() < this->maxResultSize)
	{
		this->genNextWord(0);
	}
	if (this->curWord == this->lastWord)
		this->done = true;
}

void WordGenerator::genNextWord(unsigned int i)
{
	if (this->curWord[i] == this->lastWord[i])
	{
		this->curWord[i] = this->startWord[i];
		this->genNextWord(i + 1);
	}
	else
	{
		this->curWord[i] += 1;
		password_s newPass;
		//char *temp = new char[this->curWord.size()];
		memcpy_s(newPass.password, this->curWord.size() + 1, this->curWord.c_str(), this->curWord.size() + 1);
		//newPass.password = temp;
		//newPass.size = this->curWord.size();
		this->result.push_back(newPass);
		cout << "Generated word[" << this->result.size() << "]: " << this->result[this->result.size() - 1].password << endl;
	}
}