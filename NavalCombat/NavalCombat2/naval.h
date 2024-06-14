#pragma once
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <set>
#include <sstream>
#include <ctime>    
#include <cstdlib> 

using namespace std;

//Определяются символьные константы для отображения различных состояний ячеек на игровом поле.
const char EMPTY = '_';
const char PIPE = 'O';
const char MISSED = '.';
const char WOUNDED = 'x';
const char KILLED = 'X';
const char UNKNOWN = '?';
const char IMPOSS = '-';
const char POSS = '+';
enum Direction {HORIS, VERT};
enum Status {OK, CROSS, NEAR};
const int BSIZE = 10; // Определяется размер игрового поля (10x10 ячеек).

//Определяется тип cell как пара символа (буква столбца) и целого числа (номер строки) для представления координат ячейки.
typedef pair<char, int> cell;

const cell INVALID_CELL = cell{ 'X', 99 };

class Ship
{
public:
	int ship_size; //размер корабля
	vector<cell> coord; //координаты клеток, зан. кораблём
	set<cell> hits; //множества клеток, по которым корабль был попадён
	bool killed = false; //флаг, указывающий, убит ли корабль
	bool has_cell(cell); //метод для проверки, содержит ли корабль указанную клетку
	char hit(cell); //метод для попадания по кораблю
	char cell_state(cell); //метод для определения состояния клетки корабля.
	void killed_check();
	bool near(cell c);
	Ship(vector<cell> crd): //конструктор класса Ship
		ship_size(crd.size()),
		coord(crd)
	{}
};

class Board
{
	vector<Ship> ships; //вектор кораблей на игровом поле
	set<cell> shoots; //множество клеток, по которым было произведены выстрелы
	bool expl = false; //флаг, указывающий, произошед ли взрыв
	bool display_pipes = true;
	char cell_state(cell);
	string title;
public:
	Board(bool is_human_board = true)
	{
		display_pipes = is_human_board;
		if (is_human_board)
		{
			title = "Human's board";
		}
		else
		{
			title = "Computer's board";
		}
	}
	static cell ij_to_cell(int i, int j) //статический метод для преобразования индексов в клетку
	{
		cell c{ 'A' + j, 1 + i };
		return c;
	}
	bool put_ship(Ship ship) //метод для добавления корабля на игровое поле
	{
		ships.push_back(ship);
		return true;
	}
	bool ship_nearby(cell c);
	void display_board();
	char shoot_result(cell c);
	bool check_coords(vector<cell>& cells);
	bool check_coords_2(vector<cell>& cells);
	bool all_ships_killed();
	char shoot(cell target);
};

class AutoGamer
{
	Board& board;
public:

	AutoGamer(Board& board) :
		board(board)
	{}
	set<cell> possible_shoots;
	set<cell> impossible_shoots;
	char prev_shoot_result(cell c);
	bool update_impossible_shoots(cell hit);
	bool update_possible_shoots(cell hit, AutoGamer& enemy);
	void delete_possible_shoots(cell c);

	bool hunting_mode = false;
	cell last_hit;
	void update_hunting_mode(char shot_result, cell target);
	cell generate_shoot(AutoGamer& human_gamer);
	cell get_shot_coord();
	bool auto_place_ships(int variant = 0);
	bool auto_place_ships_random();
	bool manual_choose_ship_positions();
	void place_ships();
	void imp_poss_draw();
	void invalidate_poss();
	void generate_rs_for_user();
};

bool string_to_cell(string in, cell& c);
void intro();
void pause();
bool is_cell_valid(cell c);
bool get_coords(vector<cell>& cells, int size);
bool is_cells_near(cell c1, cell c2);
cell get_random_cell();
vector<cell> ship_project(cell head, Direction dir, int size);
