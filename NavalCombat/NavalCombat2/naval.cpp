#include "naval.h"

void Board::dispay_board()
{
	cout << "   A B C D E F G H I J" << endl;
	for (int i = 0; i < BSIZE; i++)
	{
		cout << setw(2);
		cout << i + 1;
		for (int j = 0; j < BSIZE; j++)
		{
			cout << "|" << cell_state(ij_to_cell(i,j));
		}
		cout << "|" << endl;
	}
}

char Board::cell_state(cell c)
{
	for (auto s : ships)
	{
		if (s.has_cell(c))
		{
			return PIPE;
		}
	}
	return EMPTY;
}

bool Ship::has_cell(cell c)
{
	auto res = find(coord.begin(), coord.end(), c);
	if (res == coord.end())
	{
		return false;
	}
	return true;
}