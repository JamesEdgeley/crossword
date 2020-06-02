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
	grid(const grid& grid) { _grid_size = grid._grid_size; _contents = grid._contents; }
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
	grid put_in(coord mcoord, const grid& mgrid)
	{
		grid ngrid=mgrid;
		_is_in = true;
		if (std::get<2>(mcoord) == true)
		{
			for (size_t k = 0; k < _word.size(); ++k)
			{
				ngrid[std::get<0>(mcoord) + k][std::get<1>(mcoord)] = _word[k];
			}
			if (std::get<0>(mcoord) > 0)
			{
				ngrid[std::get<0>(mcoord) - 1][std::get<1>(mcoord)] = ' ';
				ngrid[std::get<1>(mcoord)][std::get<0>(mcoord) - 1] = ' ';
			}
			if (std::get<0>(mcoord) +  _word.size()< ngrid.size())
			{
				ngrid[std::get<0>(mcoord) + _word.size()][std::get<1>(mcoord)] = ' ';
				ngrid[std::get<1>(mcoord)][std::get<0>(mcoord) + _word.size()] = ' ';

			}

		}

		else if (std::get<2>(mcoord) == false)
		{
			for (size_t k = 0; k < _word.size(); ++k)
			{
				ngrid[std::get<0>(mcoord)][std::get<1>(mcoord) + k] = _word[k];
			}
			if (std::get<1>(mcoord) > 0)
			{
				ngrid[std::get<0>(mcoord)][std::get<1>(mcoord)-1] = ' ';
				ngrid[std::get<1>(mcoord) - 1][std::get<0>(mcoord)] = ' ';
			}
			if (std::get<1>(mcoord) + _word.size() < ngrid.size())
			{
				ngrid[std::get<0>(mcoord)][std::get<1>(mcoord) + _word.size()] = ' ';
				ngrid[std::get<1>(mcoord) + _word.size()][std::get<0>(mcoord)] = ' ';

			}
		}
		return ngrid;
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
		for (size_t j = 0; j < mgrid.size(); ++j)
		{
			coord across(i, j, true);

			bool aworks = true;

			for (size_t k = 0; k < _word.size(); ++k)
			{
				if (mgrid[i + k][j] != '0' && mgrid[i + k][j] != _word[k])
					aworks = false;

			}
			if (aworks == true)
				valids.push_back(across);

;

		}
	}

	for (size_t i = 0; i < mgrid.size(); ++i)
	{
		for (size_t j = 0; j < mgrid.size() - _word.size() + 1; ++j)
		{

			coord down(i, j, false);

			bool dworks = true;
			for (size_t k = 0; k < _word.size(); ++k)
			{

				if (mgrid[i][j + k] != '0' && mgrid[i][j + k] != _word[k])
					dworks = false;
			}


			if (dworks == true)
				valids.push_back(down);

		}
	}


	return valids;
}


void Fill(std::vector<word> wordlist, grid& mgrid)
{
	//std::vector<word> trial_wordlist = wordlist;
	if (wordlist.empty() == true)
	{
		//std::cout << "This should not be zero->" << wordlist.size() << std::endl;
		std::cout << "complete" << std::endl;
		mgrid.print();
		return;
	}


	else
	{
		//std::cout << "working on: " << wordlist.back() << std::endl;
		word activeword = wordlist.back();
		std::vector<coord> valids = activeword.Find_Valids(mgrid);
		if (valids.empty() == true)
		{
			wordlist.pop_back();
			Fill(wordlist, mgrid);
			
		}
		else
		{
			//std::cout << "This should not be zero->" << wordlist.size() << std::endl;
			//std::cout << "number of valids: " << valids.size() << std::endl;
			
			for (auto& valid : valids)
			{
				grid ngrid=activeword.put_in(valid, mgrid);
				std::vector<word> nwordlist = wordlist;
				nwordlist.pop_back();
				Fill(nwordlist, ngrid);
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


	const int gridsize = 4;

	grid Grid(gridsize);
	Fill(wordlist, Grid);
	/*
	word activeword = wordlist[1];
	std::vector<coord> valids = activeword.Find_Valids(Grid);
	for (auto& valid : valids)
	{
		grid newgrid = activeword.put_in(valid, Grid);

		newgrid.print();
		word newactiveword = wordlist[2];
		
		std::vector<coord> newvalids = newactiveword.Find_Valids(newgrid);
		for (auto& valid : newvalids)
		{
			grid newnewgrid=newactiveword.put_in(valid, newgrid);
			newnewgrid.print();
			std::cout << std::endl;
		}

		
	}
	*/
	//
	

	return 0;
}  