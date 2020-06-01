#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

class word
{
private:
	std::string _word;
	bool _is_in;

public:
	word(std::string word) : _word(word), _is_in(0) {};
	word() {};
	void put_in() { _is_in = true; }
	void take_out() { _is_in = false; }
	bool is_in() { return _is_in; }
	size_t size() { return _word.size(); }
	friend std::ostream& operator<<(std::ostream& os, word &word) { os << word._word; return os; }
};

int main()
{
	std::ifstream file("wordlist.txt");

	std::vector<word> wordlist;
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			word new_word(line);
			wordlist.push_back(new_word);
		}
		file.close();
	}

	std::sort(wordlist.begin(), wordlist.end(), []
	(word& first, word& second)
		{
			return first.size() > second.size();
		});
	

	for (auto word : wordlist)
	{
		std::cout << word << std::endl;
	}


	const int gridsize = 15;

	char grid[gridsize][gridsize];

	for (auto &column : grid)
	{
		for (char& letter : column)
		{
			letter='0';
		}
	}



	for (auto &column: grid)
	{
		for (auto &letter : column)
		{
			//std::cout << letter;
		}
		//std::cout << std::endl;
	}
	

	return 0;
}