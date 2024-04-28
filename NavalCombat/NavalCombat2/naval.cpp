#include "naval.h"

void Board::display_board()
{
	cout << "   A B C D E F G H I J" << endl;
	for (int i = 0; i < BSIZE; i++)
	{
		cout << setw(2);
		cout << i + 1;
		for (int j = 0; j < BSIZE; j++)
		{
			cout << "|" << cell_state(ij_to_cell(i, j));
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
	return EMPTY;
}

bool Ship::has_cell(cell c) //определение метода has_cell класса Ship, который проверяет, содержит ли корабль указанную клетку
{
	auto res = find(coord.begin(), coord.end(), c); //ищет клетку в векторе координат корабля
	if (res == coord.end())
	{
		return false; //если клетка не найдена, возвращает false
	}
	return true;
}

char Board::shoot(cell target) //определение метода shoot класса Board, который обрабатывает попадание по цели
{
	shoots.insert(target); //добавляет цель в множество попавших по цели
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
	return hit ? cell_state(target) : MISSED; //возвращает состояние цели после попадания
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

void AutoGamer::place_ships()
{
	vector<vector<cell>> ship_positions =
	{
		{{'A', 1}, {'A', 2}, {'A', 3}, {'A', 4}}, 
		{{'C', 1}, {'C', 2}, {'C', 3}}, 
		{{'D', 5}, {'D', 6}},
		{{'E', 3}, {'E', 4}}, 
	};

	for (const auto& ship_position : ship_positions)
	{
		Ship ships(ship_position.size(), ship_position);
		board.put_ship(ships);
	}
}
cell AutoGamer::generate_shoot()
{
	int i = rand() % BSIZE;
	int j = rand() % BSIZE;
	return board.ij_to_cell(i, j);
}