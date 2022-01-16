#include "Interface.h"
#include <fstream>

void WordsMultiset::add(const std::string& word, size_t times)
{
	for (size_t i = 0; i < times; i++)
	{
		this->words_fill.push_back(word);
		this->size++;
	}
}

size_t WordsMultiset::get_size() const
{
	return this->size;
}

void WordsMultiset::remove(const string& word)
{
	size_t size = this->words_fill.get_size();
	for (size_t i = 0; i < size; i++)
	{
		if (this->words_fill[i] == word)
		{
			this->words_fill.pop_at(i);
			this->size--;
			return;
		}
	}
}

bool WordsMultiset::contains(const std::string& word) const
{
	size_t size = this->words_fill.get_size();
	for (size_t i = 0; i < size; i++)
		if (this->words_fill[i] == word)
			return true;

	return false;
}

size_t WordsMultiset::countOf(const std::string& word) const
{
	size_t count = 0;
	size_t size = this->words_fill.get_size();
	for (size_t i = 0; i < size; i++)
		if (this->words_fill[i] == word)
			count++;

	return count;
}

size_t WordsMultiset::countOfUniqueWords() const
{
	size_t count = 0;
	size_t size = this->words_fill.get_size();
	Vector<string> uniques;
	for (size_t i = 0; i < size; i++)
	{
		if (!uniques.contains(this->words_fill[i]))
		{
			uniques.push_back(this->words_fill[i]);
			count++;
		}
	}

	return count;
}

std::multiset<std::string> WordsMultiset::words() const
{
	std::multiset<string> set;
	size_t size = this->words_fill.get_size();
	for (size_t i = 0; i < size; i++)
		set.insert(this->words_fill[i]);

	return set;
}

void WordsMultiset::print() const
{
	size_t size = this->words_fill.get_size();
	for (size_t i = 0; i < size; i++)
		std::cout << this->words_fill[i] << " ";

	std::cout << std::endl;
}

Vector<string> fill_vector_from_string(const string& list)
{
	Vector<string> words_list;
	string word;
	size_t sizeA = list.size();
	for (size_t i = 0; i < sizeA; i++)
	{
		if (list[i] == ' ' || list[i] == '\n')
			continue;

		word += list[i];
		if (list[i + 1] == '\0')
		{
			words_list.push_back(word);
			break;
		}

		if (list[i + 1] == ' ' || list[i + 1] == '\n')
		{
			words_list.push_back(word);
			word = "";
		}
	}
	
	return words_list;
}

ComparisonReport Comparator::compare(std::istream& a, std::istream& b)
{
	char symbol{};
	string file_words{};
	Vector<string> words_list;
	size_t size;
	while (!a.eof())
	{
		a.get(symbol);
		if (symbol == '\0')
			continue;

		file_words += symbol;
	}
	if (file_words.size() - 1 < file_words.size())
		file_words[file_words.size() - 1] = '\0';

	else
		file_words[file_words.size()] = '\0';

	words_list = fill_vector_from_string(file_words);
	size = words_list.get_size();
	for (size_t i = 0; i < size; i++)
		this->report.uniqueWords[0].add(words_list[i]);

	char symbol2{};
	file_words = "";
	words_list.clear();
	while (!b.eof())
	{
		b.get(symbol2);
		if (symbol2 == '\0')
			continue;

		file_words += symbol2;
	}
	if (file_words.size() - 1 < file_words.size())
		file_words[file_words.size() - 1] = '\0';

	else
		file_words[file_words.size()] = '\0';

	words_list = fill_vector_from_string(file_words);
	size = words_list.get_size();
	for (size_t i = 0; i < size; i++)
	{
		if (this->report.uniqueWords[0].contains(words_list[i]))
		{
			this->report.uniqueWords[0].remove(words_list[i]);
			this->report.commonWords.add(words_list[i]);
			continue;
		}

		this->report.uniqueWords[1].add(words_list[i]);
	}

	std::cout << "File 1 has " << this->report.uniqueWords[0].get_size() + this->report.commonWords.get_size() << " words";
	std::cout << " and " << this->report.commonWords.get_size() << " are in File 2. ";
	if (this->report.uniqueWords[0].get_size() + this->report.commonWords.get_size() != 0)
		std::cout << "(" << floor(this->report.commonWords.get_size() * 100 / (this->report.uniqueWords[0].get_size() + this->report.commonWords.get_size())) << "%)" << std::endl;
	else
		std::cout << "(0%)" << std::endl;

	std::cout << "File 2 has " << this->report.uniqueWords[1].get_size() + this->report.commonWords.get_size() << " words";
	std::cout << " and " << this->report.commonWords.get_size() << " are in File 1. ";
	if (this->report.uniqueWords[1].get_size() + this->report.commonWords.get_size() != 0)
		std::cout << "(" << floor(this->report.commonWords.get_size() * 100 / (this->report.uniqueWords[1].get_size() + this->report.commonWords.get_size())) << "%)" << std::endl;
	else
		std::cout << "(0%)" << std::endl;

	return this->report;
}

string get_string(const string& list)
{
	size_t size = list.size();
	string file_path;
	for (size_t i = 0; i < size; i++)
	{
		if (list[i] == ' ')
			continue;

		file_path += list[i];
	}

	return file_path;
}

int main()
{
	string file_name1, file_name2;
	string file1, file2;
	std::cout << "Input File 1 name or directory: ";
	std::cin >> file_name1;
	file1 = get_string(file_name1);
	std::cout << std::endl;
	std::ifstream file1_data(file1, std::ios::beg);
	if (!file1_data.is_open())
	{
		std::cout << "File 1 did not open." << std::endl;
		return 0;
	}

	std::cout << "Input File 2 name or directory: ";
	std::cin >> file_name2;
	file2 = get_string(file_name2);
	std::cout << std::endl;
	std::ifstream file2_data(file2, std::ios::beg);
	if (!file2_data.is_open())
	{
		std::cout << "File 2 did not open." << std::endl;
		return 0;
	}

	Comparator c;
	ComparisonReport report = c.compare(file1_data, file2_data);
	
	return 0;
}
