#pragma once
#include <set>
#include <string>
#include <sstream>
#include "Vector.h"

using std::string;

class WordsMultiset 
{
private:
	Vector<string> words_fill;
	size_t size{};
public:
	size_t get_size() const;
	void add(const std::string& word, size_t times = 1);
	void remove(const string& word);
	bool contains(const std::string& word) const;
	size_t countOf(const std::string& word) const;
	size_t countOfUniqueWords() const;
	std::multiset<std::string> words() const;
	void print() const;
};

class ComparisonReport 
{
public:
	WordsMultiset commonWords;
	WordsMultiset uniqueWords[2];
};

class Comparator 
{
private:
	ComparisonReport report;
public:
	ComparisonReport compare(std::istream& a, std::istream& b);
};