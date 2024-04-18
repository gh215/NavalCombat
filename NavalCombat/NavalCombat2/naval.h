#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

const char EMPTY = '_';
const char PIPE = 'O';
const int BSIZE = 10;

typedef pair<char, int> cell;

class Ship
{
public:
	int ship_size;
	vector<cell> coord;
	bool has_cell(cell);
	Ship(int size, vector<cell> crd):
		ship_size(size),
		coord(crd)
	{}
};

class Board
{
	vector<Ship> ships;
	vector<cell> shoots;
	bool expl = false;
public:
	static cell ij_to_cell(int i, int j)
	{
		cell c{ 'A' + j, 1 + i };
		return c;
	}
	bool put_ship(Ship ship)
	{
		ships.push_back(ship);
		return true;
	}
	void dispay_board();
	char cell_state(cell);
};
