#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <set>

using namespace std;

//Определяются символьные константы для отображения различных состояний ячеек на игровом поле.
const char EMPTY = '_';
const char PIPE = 'O';
const char MISSED = '.';
const char WOUNDED = 'x';
const char KILLED = 'X';
const int BSIZE = 10; // Определяется размер игрового поля (10x10 ячеек).

//Определяется тип cell как пара символа (буква столбца) и целого числа (номер строки) для представления координат ячейки.
typedef pair<char, int> cell;

//
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
	Ship(int size, vector<cell> crd): //конструктор класса Ship
		ship_size(size),
		coord(crd)
	{}
};

class Board
{
	vector<Ship> ships; //вектор кораблей на игровом поле
	set<cell> shoots; //множество клеток, по которым было произведено попадание
	bool expl = false; //флаг, указывающий, произошед ли взрыв
public:
	bool is_human_board = true;
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
	void display_board();
	char shoot(cell target);
	char cell_state(cell);
};

class AutoGamer
{
public:
	Board& board; 
	AutoGamer(Board& board) : board(board) {} // Конструктор принимает ссылку на Board
	void place_ships();
	cell generate_shoot();
};
