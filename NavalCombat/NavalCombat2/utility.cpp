#include "naval.h"


void intro()
{
	cout << "\tДобро пожаловать в игру \"Морской бой\"!" << endl;
	cout << "Ваша задача - уничтожить все корабли противника." << endl;
	cout << "Корабли размещаются на игровом поле 10x10." << endl;
	cout << "Корабли могут быть следующих размеров: 1, 2, 3, 4 палубы." << endl;
	cout << "Игра начинается с размещения кораблей." << endl;
	cout << "Ход игрока начинается с выстрела по координатам." << endl;
	cout << "Если попал в корабль или убил, то ход не сменяется." << endl;
	cout << "Игра продолжается до тех пор, пока один из игроков не уничтоит все корабли противника." << endl;
	cout << "Удачи!" << endl;
}

bool is_cell_valid(cell c)
{
	if ((c.first >= 'A' && c.first <= 'J') && (c.second >= 1 && c.second <= 10))
	{
		return true;
	}
	return false;
}

bool string_to_cell(string in, cell& c)
{
	if (in.size() != 2 && in.size() != 3) return false;

	char col = toupper(in[0]);
	if (col < 'A' || col > 'J') return false;
	int row = stoi(in.substr(1));
	if (row < 1 || row > 10) return false;

	c = cell(col, row);
	return true;
}

bool get_coords(vector<cell>& cells, int size)
{
	string input;
	cout << "Введите координаты корабля размера: " << size << "(например, A1 B1 C1)" << endl;
	getline(cin, input);
	istringstream str(input);
	string word;

	while (str >> word)
	{
		cell c;
		if (!string_to_cell(word, c))
		{
			return false;
		}
		cells.push_back(c);
	}
	if (cells.size() != size)
	{
		return false;
	}
	return true;
}

bool is_cells_near(cell c1, cell c2)
{
	if (abs(c1.first - c2.first) > 1 || abs(c1.second - c2.second) > 1)
	{
		return false;
	}
	if (c1.first == c2.first && c1.second == c2.second)
	{
		return false;
	}
	return true;
}

bool check_coords(vector<cell>& cells, Board& board)
{
	// Проверяем, что координаты не пересекаются с уже размещенными кораблями
	for (auto c : cells)
	{
		if (board.cell_state(c) != EMPTY)
		{
			cout << "Координаты пересекаются с уже размещенным кораблем." << endl;
			return false;
		}
		if (board.ship_nearby(c))
		{
			cout << "Корабль не может быть размещён рядом с другим кораблём." << endl;
			return false;
		}
	}
	return true;
}

