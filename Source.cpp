#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<tuple>


typedef std::tuple<int, int, bool> coord;


class grid
{
private:
	size_t _grid_size;
	std::vector<std::vector<char>> _contents;
public:
	grid(int grid_size) : _grid_size(grid_size), _contents(grid_size, std::vector<char>(grid_size, '0')) {};
	std::vector<char>& operator[](int i) { return _contents[i]; }
	size_t size() { return _grid_size; }
	void print()
	{
		for (auto& column : _contents)
		{
			for (auto& letter : column)
			{
				std::cout << letter;
			}
			std::cout << std::endl;
		}
	}
	std::vector<std::vector<char>>::iterator begin() { return _contents.begin(); }
	std::vector<std::vector<char>>::iterator end() { return _contents.end(); }
};


class word
{
private:
	std::string _word;
	bool _is_in;
	std::vector<coord>  _valid_coords;

public:
	word(std::string word) : _word(word), _is_in(0) {};
	void put_in(coord mcoord, grid& mgrid)
	{
		_is_in = true;
		if (std::get<2>(mcoord) == true)
		{
			for (size_t k = 0; k < _word.size(); ++k)
			{
				mgrid[std::get<0>(mcoord) + k][std::get<1>(mcoord)] = _word[k];
			}
			if (std::get<0>(mcoord) > 0)
			{
				mgrid[std::get<0>(mcoord) - 1][std::get<1>(mcoord)] = ' ';
				mgrid[std::get<1>(mcoord)][std::get<0>(mcoord) - 1] = ' ';
			}
			if (std::get<0>(mcoord) +  _word.size()< mgrid.size())
			{
				mgrid[std::get<0>(mcoord) + 1][std::get<1>(mcoord)] = ' ';
				mgrid[std::get<1>(mcoord)][std::get<0>(mcoord) + 1] = ' ';

			}

		}

		else if (std::get<2>(mcoord) == false)
		{
			for (size_t k = 0; k < _word.size(); ++k)
			{
				mgrid[std::get<0>(mcoord)][std::get<1>(mcoord) + k] = _word[k];
			}
			if (std::get<1>(mcoord) > 0)
			{
				mgrid[std::get<0>(mcoord)][std::get<1>(mcoord)-1] = ' ';
				mgrid[std::get<1>(mcoord) - 1][std::get<0>(mcoord)] = ' ';
			}
			if (std::get<1>(mcoord) + _word.size() < mgrid.size())
			{
				mgrid[std::get<0>(mcoord)][std::get<1>(mcoord) + 1] = ' ';
				mgrid[std::get<1>(mcoord) + 1][std::get<0>(mcoord)] = ' ';

			}
		}
	}
	void take_out() { _is_in = false; }
	bool is_in() { return _is_in; }
	size_t size() { return _word.size(); }
	friend std::ostream& operator<<(std::ostream& os, word &word) { os << word._word; return os; }

	std::vector<coord> Find_Valids(grid& mgrid);
};


std::vector<coord> word::Find_Valids(grid& mgrid)
{
	std::vector<coord> valids;
	for (size_t i = 0; i < mgrid.size() - _word.size() + 1; ++i)
	{
		for (size_t j = 0; j < mgrid.size() - _word.size() + 1 ; ++j)
		{
			coord across(i, j, true);
			coord down(i, j, false);
			bool aworks = true;
			bool dworks = true;
			for (size_t k = 0; k < _word.size(); ++k)
			{
				if (mgrid[i + k][j] != '0' && mgrid[i + k][j] != _word[k])
					aworks = false;
				if (mgrid[i][j + k] != '0' && mgrid[i][j+k] != _word[k])
					dworks = false;
			}
			if (aworks == true)
				valids.push_back(across);

			if (dworks == true)
				valids.push_back(down);

		}
	}
	return valids;
}


void Fill(std::vector<word> wordlist, grid& mgrid)
{
	if (wordlist.size() == 0)
	{
		std::cout << "complete" << std::endl;
		//mgrid.print();
	}
	else if (wordlist.size()!=0)
	{
		//std::cout << "working on: " << wordlist.back() << std::endl;
		std::vector<coord> valids = wordlist.back().Find_Valids(mgrid);
		if (valids.size() == 0)
		{
			//std::cout << "no valids" << std::endl;
			//mgrid.print();
		}
		else
		{
			//std::cout << "number of valids: " << valids.size() << std::endl;
			for (auto& valid : valids)
			{
				std::cout <<"This should not be zero->"<< wordlist.size() << std::endl;
				wordlist.back().put_in(valid, mgrid);
				wordlist.pop_back();
				Fill(wordlist, mgrid);
			}
		}
	}
	
}


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

	std::sort(wordlist.begin(), wordlist.end(), [](word& first, word& second)
		{
			return first.size() < second.size();
		});
	
	for (auto word : wordlist)
	{
		//std::cout << word << std::endl;
	}


	const int gridsize = 15;

	grid Grid(gridsize);
	//Grid[1][3] = 'T';
	//Grid[2][2] = 'T';
	//Grid[2][0] = 'C';
	//std::cout << Grid.size() << " " << wordlist[0].size();
	//std::cout << wordlist.back();
	Fill(wordlist, Grid);
	
	//std::vector<coord> valids = wordlist[0].Find_Valids(Grid);

	//Grid.print();
	

	return 0;
}