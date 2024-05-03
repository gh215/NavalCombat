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

void Board::display_board()
{
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

void AutoGamer::place_ships_computer()
{
	vector<vector<cell>> ship_positions =
	{
		{{'A', 1}, {'A', 2}, {'A', 3}, {'A', 4}},
		{{'C', 1}, {'C', 2}, {'C', 3}},
		{{'G', 1}, {'G', 2}, {'G', 3}},
		{{'F', 5}, {'G', 5}},
		{{'E', 6}, {'F', 6}},
		{{'B', 6}, {'C', 6}},
		{{'A', 10}},
		{{'B', 8}},
		{{'F', 7}},
		{{'E', 9}},
	};

	for (const auto& ship_position : ship_positions)
	{
		Ship ships(ship_position);
		board.put_ship(ships);
	}
}

cell AutoGamer::generate_shoot()
{
	int i = rand() % BSIZE;
	int j = rand() % BSIZE;
	return board.ij_to_cell(i, j);
}

cell AutoGamer::get_shot_coord()
{
	string input;
	char column;
	int row;

	while (true)
	{
		cout << "Введите координаты выстрела: ";
		cin >> input;

		if (input.size() == 2)
		{
			column = tolower(input[0]); // Преобразование в нижний регистр для удобства сравнения
			row = input[1] - '0'; // Преобразование символа в число

			if ((column >= 'a' && column <= 'j') && (row >= 1 && row <= 10))
			{
				return make_pair(column, row);
			}
		}
		cout << "Неверный формат. Попробуйте еще раз." << endl;
	}
}

void AutoGamer::place_ships_human()
{
	vector<int> ship_sizes = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	int total_ships = 10;
	int placed_ships = 0;

	while (placed_ships < total_ships)
	{
		cout << "Введите координаты для размещения корабля размером " << ship_sizes[placed_ships] << " (например, A1 B1 C1): ";
		string input;
		cin >> input;

		vector<cell> ship_coords;
		int ship_size = ship_sizes[placed_ships];
		//Для каждого корабля размера ship_sizes[placed_ships] выполняется цикл, в котором из ввода извлекаются координаты.
		for (int i = 0; i < ship_size; i++)
		{
			//Это выражение, которое вычисляет позицию следующего символа в строке input. 
			//Мы умножаем индекс на 2, чтобы получить позицию следующего символа.
			int next_symbol_pos = 2 * (i + 1);
			int current_symbol_pos = 2 * i;
			if (input.size() < next_symbol_pos)
			{
				cout << "Недостаточно координат. Попробуйте еще раз." << endl;
				break;
			}

			char column = tolower(input[current_symbol_pos]);
			int row = input[next_symbol_pos] - '0';

			if (!((column >= 'a' && column <= 'j') && (row >= 1 && row <= 10)))
			{
				cout << "Неверный формат координат. Попробуйте еще раз." << endl;
				break;
			}
			ship_coords.push_back(make_pair(column, row));
		}

		if (!ship_coords.empty())
		{
			Ship ship(ship_coords);
			board.put_ship(ship);
			placed_ships++;
		}
	}
}