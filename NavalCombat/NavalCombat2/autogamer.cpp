#include "naval.h"


cell AutoGamer::generate_shoot()
{
	if (hunting_mode && possible_shoots.size() != 0)
	{
		return *possible_shoots.begin();
	}

	while (true) // Добавляем цикл, чтобы гарантировать выбор допустимой клетки
	{
		int i = rand() % BSIZE;
		int j = rand() % BSIZE;
		cell c = board.ij_to_cell(i, j);

		// Проверяем, не стреляли ли уже по этой клетке и не содержится ли она в impossible_shoots
		if (board.cell_state(c) == EMPTY && impossible_shoots.find(c) == impossible_shoots.end())
		{
			return c;
		}
	}
}

//функция обновляет статус режима охоты в зависимости от результата последнего выстрела 
void AutoGamer::update_hunting_mode(char shot_result, cell target)
{
	if (shot_result == WOUNDED)
	{
		hunting_mode = true;
		//last_hit обновляется на координаты последнего попадания
		last_hit = target;
	}
	else if (shot_result == KILLED)
	{
		hunting_mode = false;
	}
}

cell AutoGamer::get_shot_coord()
{
	string input;
	while (true)
	{
		cout << "Введите координаты выстрела (или 'q' для выхода): ";
		getline(cin, input);

		if (input == "q" || input == "Q")
		{
			exit(0);
		}
		cell c;
		if (string_to_cell(input, c))
		{
			return c;
		}
	}
}

bool AutoGamer::auto_place_ships(int variant)
{
	vector<vector<cell>> ship_positions0 =
	{
		{{'A', 1}, {'A', 2}, {'A', 3}, {'A', 4}},
		{{'C', 1}, {'C', 2}, {'C', 3}},
		{{'G', 1}, {'G', 2}, {'G', 3}},
		{{'F', 5}, {'G', 5}},
		{{'D', 5}, {'D', 6}},
		{{'B', 8}, {'B', 9}},
		{{'F', 10}},
		{{'D', 9}},
		{{'F', 7}},
		{{'I', 7}},
	};

	vector<vector<cell>> ship_positions1 =
	{
		{{'I', 1}, {'I', 2}, {'I', 3}, {'I', 4}},
		{{'D', 5}, {'D', 6}, {'D', 7}},
		{{'A', 10}, {'B', 10}, {'C', 10}},
		{{'B', 2}, {'C', 2}},
		{{'F', 1}, {'F', 2}},
		{{'H', 7}, {'H', 8}},
		{{'A', 5}},
		{{'A', 8}},
		{{'G', 5}},
		{{'J', 10}},
	};

	vector<vector<cell>> ship_positions;

	switch (variant)
	{
	case 0:
		ship_positions = ship_positions0;
		break;
	case 1:
		ship_positions = ship_positions1;
		break;
	default:
		cout << "Недопустимый номер варианта ship posiions" << endl;
		return false;
	}

	for (const auto& ship_position : ship_positions)
	{
		Ship ships(ship_position);
		board.put_ship(ships);
	}
	return true;
}

bool AutoGamer::manual_place_ships()
{
	vector<int> ships_sizes{ 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	for (int i = 0; i < ships_sizes.size(); i++)
	{
		vector<cell> cells;
		//Принимает ввод пользователя, пытается преобразовать его в вектор cell
		if (!get_coords(cells, ships_sizes[i]))
		{
			return false;
		}

		//Проверяет возможность размещения корабля в board
		if (!check_coords(cells, board))
		{
			cout << "Не удалось разместить корабль. Попробуйте еще раз." << endl;
			return false;
		}
		board.put_ship(Ship(cells));
	}
	return true;
}

bool AutoGamer::manual_choose_ship_positions()
{
	vector<vector<cell>> ship_positions;
	vector<int> ships_sizes{ 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

	for (int size : ships_sizes)
	{
		while (true)
		{
			vector<cell> cells;
			if (get_coords(cells, size) && check_coords(cells, board))
			{
				board.put_ship(Ship(cells));
				break;
			}
			else
			{
				cout << "Неправильные координаты, попробуйте снова." << endl;
			}
		}
	}
	return true;
}

char AutoGamer::prev_shoot_result(cell c)
{
	char res = board.cell_state(c);

	if (res == MISSED || res == WOUNDED || res == KILLED)
	{
		return res;
	}
	return UNKNOWN;
}

bool AutoGamer::update_impossible_shoots(cell hit)
{
	if (!is_cell_valid(hit)) return false;
	vector<pair<int, int>> offsets = { {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };

	for (auto offset : offsets)
	{
		cell imp = hit;
		imp.first += offset.first;
		imp.second += offset.second;
		if (!is_cell_valid(imp)) continue;
		impossible_shoots.insert(imp);
	}
	return true;
}

bool AutoGamer::update_possible_shoots(cell hit, AutoGamer& enemy)
{
	if (!is_cell_valid(hit)) return false;
	vector<pair<int, int>> offsets = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	for (auto offset : offsets)
	{
		cell poss = hit;
		poss.first += offset.first;
		poss.second += offset.second;
		if (!is_cell_valid(poss)) continue;
		if (impossible_shoots.count(poss) == 1) continue;
		char shot_res = enemy.prev_shoot_result(poss);
		if (shot_res == MISSED || shot_res == KILLED || shot_res == WOUNDED) continue;
		possible_shoots.insert(poss);
	}
	return true;
}

void AutoGamer::delete_possible_shoots(cell c)
{
	//possible_shoots.erase(c);
	impossible_shoots.insert(c);
}

bool AutoGamer::auto_place_ships_random()
{
	srand(static_cast<unsigned int>(time(0)));
	vector<int> ships_sizes{ 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

	//цикл будет выполнятся, пока корабль не будет успешно размещен на поле и все корабли не будут размещены на поле
	for (int size : ships_sizes)
	{
		bool placed = false;

		while (!placed)
		{
			//случайное направление корабля
			bool horizontal = rand() % 2; // 0 - горизонтально, 1 - вертикально
			//генерация начальных коорд. с учётом границ поля
			/*
			Если корабль располагается горизонтально (horizontal == true), то row генерируется в диапазоне от 1 до BSIZE,
			а col — в диапазоне от 'A' до 'A' + BSIZE - size.
			*/
			int row = horizontal ? rand() % BSIZE + 1 : rand() % (BSIZE - size + 1) + 1;
			/*
			Если корабль располагается вертикально (horizontal == false), 
			то row генерируется в диапазоне от 1 до BSIZE - size + 1, а col — в диапазоне от 'A' до 'A' + BSIZE.
			*/
			int col = horizontal ? rand() % (BSIZE - size + 1) + 'A' : rand() % BSIZE + 'A';
			//пустой вектор для хранение ячеек корабля
			vector<cell> cells;

			//цикл, заполняет вектор cells координатами ячеек корабля в зависимости от его напр. и размера
			for (int i = 0; i < size; ++i)
			{
				if (horizontal)
				{
					cells.push_back(cell{ static_cast<char>(col + i), row });
				}
				else {
					cells.push_back(cell{ col, row + i });
				}
			}

			// проверка на возможность размещения корабля в данных координатах
			if (check_coords(cells, board))
			{
				board.put_ship(Ship(cells));
				placed = true;
			}
		}
	}
	return true;
}