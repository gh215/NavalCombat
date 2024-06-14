#include "naval.h"


cell AutoGamer::generate_shoot(AutoGamer& human_gamer)
{
	if (hunting_mode && possible_shoots.size() != 0)
	{
		return *possible_shoots.begin();
	}

	set<cell> shot;

	while (shot.size() != BSIZE * BSIZE)
	{
		cell c = get_random_cell();
		shot.insert(c);

		if (impossible_shoots.count(c) == 1) continue;
		// туда куда не стрелял и для какой клетки не выяснил невозможность

		if (human_gamer.prev_shoot_result(c) != UNKNOWN) continue;
		return c;
	}
}

//функция обновляет статус режима охоты в зависимости от результата последнего выстрела 
void AutoGamer::update_hunting_mode(char shot_result, cell target)
{
	// изменить все poss на imposs shoots и добавить impossible shoots вокруг убившей клетки
	if (shot_result == WOUNDED)
	{
		hunting_mode = true;
		//last_hit обновляется на координаты последнего попадания
		last_hit = target;
	}
	else if (shot_result == KILLED)
	{
		hunting_mode = false;

		/**/// Переносим все возможные выстрелы в невозможные
		for (auto pos : possible_shoots)
		{
			impossible_shoots.insert(pos);
		}
		possible_shoots.clear();

		vector<pair<int, int>> offsets = { {-1, -1}, {1, -1}, {-1, 1}, {1, 1}, {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
		for (auto offset : offsets)
		{
			/*if (is_cell_valid(around_target))
			{
				impossible_shoots.insert(around_target);
			}
			*/
		}
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

bool AutoGamer::manual_choose_ship_positions()
{
	vector<vector<cell>> ship_positions;
	vector<int> ships_sizes{ 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

	for (int size : ships_sizes)
	{
		while (true)
		{
			vector<cell> cells;
			cout << "Введите координаты корабля размера " << size << " (например, A1 B1 C1): ";
			if (get_coords(cells, size) && board.check_coords(cells))
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
	return board.shoot_result(c);
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
		delete_possible_shoots(imp);
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
	possible_shoots.erase(c);
}

bool AutoGamer::auto_place_ships_random()
{
	vector<int> ships_sizes{ 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

	//цикл будет выполнятся, пока корабль не будет успешно размещен на поле и все корабли не будут размещены на поле
	for (int size : ships_sizes)
	{
		bool placed = false;

		//for (int tries = 0; tries < MAX_TRIES; tries++)
		// когда корабль поставлен, break
		while (!placed)
		{
			Direction dir = VERT;

			bool horizontal = rand() % 2; // 0 - горизонтально, 1 - вертикально

			if (horizontal)
			{
				dir = HORIS;
			}

			cell head = get_random_cell();

			//вектор для хранение ячеек корабля
			vector<cell> cells = ship_project(head, dir, size);

			// проверка на возможность размещения корабля в данных координатах
			if (board.check_coords(cells))
			{
				board.put_ship(Ship(cells));
				placed = true;
			}
		}
	}
	return true;
}

void AutoGamer::place_ships()
{
	cout << "Выберите способ расстановки кораблей:" << endl;
	cout << "1. Автоматическая расстановка" << endl;
	cout << "2. Выбор из трёх автоматически сгенерированных полей" << endl;
	cout << "3. Вручную" << endl;

	int choice;
	cout << "Ваш выбор: ";
	cin >> choice;

	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	switch (choice)
	{
	case 1:
		auto_place_ships_random();
		break;
	case 2:
		generate_rs_for_user();
		break;
	case 3:
		if (!manual_choose_ship_positions())
		{
			exit(0);
		}
		break;
	default:
		cout << "Неверный выбор, попробуйте снова." << endl;
		return;
	}
}

void AutoGamer::imp_poss_draw()
{
	for (int i = 0; i < BSIZE; i++)
	{
		cout << "   ";
		for (int j = 0; j < BSIZE; j++)
		{
			char disp = '.';
			cell c = Board::ij_to_cell(i, j);
			if (possible_shoots.count(c)) disp = POSS;
			if (impossible_shoots.count(c)) disp = IMPOSS;
			if (possible_shoots.count(c) && impossible_shoots.count(c)) disp = '!';
			cout << disp << " ";
		}
		cout << endl;
	}
}

void AutoGamer::invalidate_poss()
{
	for (cell c : possible_shoots)
	{
		impossible_shoots.insert(c);
	}
	for (cell c : impossible_shoots)
	{
		possible_shoots.erase(c);
	}
}

void AutoGamer::generate_rs_for_user()
{
	while (true)
	{
		board = Board(true); // Создаем новую доску перед генерацией
		auto_place_ships_random();
		system("cls");
		cout << "\nВарианты игровых полей: " << endl;
		board.display_board();

		int user_choice;
		cout << "Вас устраивает этот вариант? Нажмите 1 для подтверждения, 2 для перезагрузки, 0 для выхода из игры: ";
		cin >> user_choice;

		if (user_choice == 0)
		{
			cout << "Выход из игры." << endl;
			exit(0);
		}
		else if (user_choice == 1)
		{

			break;
		}
		else if (user_choice == 2)
		{
			continue;
		}
		else
		{
			cout << "Извините, но это неправильная клавиша." << endl;
		}
	}
}