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
	cout << "Введите координаты корабля размера: " << size << "(например, A1 B1 C1): ";
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

cell get_random_cell()
{
	Board board;
	int i = rand() % BSIZE;
	int j = rand() % BSIZE;
	cell c = board.ij_to_cell(i, j);
	return c;
}

vector<cell> ship_project(cell head, Direction dir, int size)
{
	vector<cell> ship_pos;
	if (dir == HORIS)
	{
		for (int i = 0; i < size; i++)
		{
			ship_pos.push_back(head);
			head.first += 1;
		}
	}
	if (dir == VERT)
	{
		for (int i = 0; i < size; i++)
		{
			ship_pos.push_back(head);
			head.second += 1;
		}
	}

	return ship_pos;
}

void pause()
{
	cout << "Нажмите Enter, чтобы продолжить...";
	cin.get();
}

