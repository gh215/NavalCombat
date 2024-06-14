#include "naval.h"

void Board::display_board()
{
	cout << endl << title << endl;
	cout << "   A B C D E F G H I J" << endl;
	for (int i = 0; i < BSIZE; i++)
	{
		cout << setw(2);
		cout << i + 1;
		for (int j = 0; j < BSIZE; j++)
		{
			//KILLED, MISSED, WOUNDED, EMPTY, PIPE
			char cs = cell_state(ij_to_cell(i, j));
			if (!display_pipes && cs == PIPE)
			{
				cs = EMPTY;
			}
			cout << "|" << cs;
		}
		cout << "|" << endl;
	}
}

char Board::cell_state(cell c)
{
	for (auto s : ships) //цикл по всем кораблям на игровом поле
	{
		if (s.has_cell(c)) //проверяет, содержит ли текущий корабль указанную клетку
		{
			return s.cell_state(c); //если клетка содержится в корабле, возвращает его состояние
		}
	}
	if (shoots.find(c) != shoots.end())
	{
		return MISSED;
	}
	return EMPTY;
}

char Board::shoot(cell target) //определение метода shoot класса Board, который обрабатывает попадание по цели
{
	shoots.insert(target);
	bool hit = false;
	for (auto& s : ships)
	{
		if (s.has_cell(target)) // проверяет, содержит ли текущий корабль цель
		{
			s.hit(target);
			hit = true;
			break;
		}
	}

	// достаточно вызвать cell_state и проан. его результаты
	return hit ? cell_state(target) : MISSED; //возвращает состояние цели после попадания
}

bool Board::check_coords(vector<cell>& cells)
{
	if (check_coords_2(cells))
	{
		return true;
	}
	return false;
}

bool Board::check_coords_2(vector<cell>& cells)
{
	// Проверяем, что координаты не пересекаются с уже размещенными кораблями
	for (auto c : cells)
	{
		if (!is_cell_valid(c))
		{
			cout << "Клетка не расположена на доске." << endl;
			return false;
		}
		if (cell_state(c) != EMPTY)
		{
			cout << "Координаты пересекаются с уже размещенным кораблем." << endl;
			return false;
		}
		if (ship_nearby(c))
		{
			cout << "Корабль не может быть размещён рядом с другим кораблём." << endl;
			return false;
		}
	}
	return true;
}

bool Board::all_ships_killed()
{
	for (const auto& ship : ships)
	{
		if (!ship.killed)
		{
			return false;
		}
	}
	return true;
}

bool Board::ship_nearby(cell c)
{
	for (auto s : ships)
	{
		if (s.near(c)) return true;
	}
	return false;
}

char Board::shoot_result(cell c)
{
	if (shoots.count(c))
	{
		return cell_state(c);
	}
	return UNKNOWN;
}
