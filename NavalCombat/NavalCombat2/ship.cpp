#include "naval.h"

bool Ship::has_cell(cell c) //определение метода has_cell класса Ship, который проверяет, содержит ли корабль указанную клетку
{
	auto res = find(coord.begin(), coord.end(), c); //ищет клетку в векторе координат корабля
	if (res == coord.end())
	{
		return false; //если клетка не найдена, возвращает false
	}
	return true;
}

//Возвращет KILLED, если корабль был убит, WOUNDED, если ранен и EMPTY, если ничего нет
char Ship::hit(cell c)
{
	if (!has_cell(c))
	{
		return EMPTY;
	}
	hits.insert(c);
	killed_check();
	return cell_state(c);
}

char Ship::cell_state(cell c)
{
	if (!has_cell(c))
	{
		return EMPTY;
	}
	if (killed)
	{
		return KILLED;
	}
	if (hits.count(c) != 0) //если клетка попала
	{
		return WOUNDED;
	}
	return PIPE;
}

void Ship::killed_check()
{
	if (hits.size() == ship_size) //если количество клеток, которые занимает корабль равно кол-во клеток, занимаемый кораблём, то он считается уничтоженным
	{
		killed = true;
	}
}

bool Ship::near(cell c)
{
	for (auto crd : coord)
	{
		if (is_cells_near(c, crd) && !has_cell(c))
		{
			return true;
		}
	}
	return false;
}