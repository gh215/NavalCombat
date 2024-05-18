#pragma once
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <set>
#include <sstream>

using namespace std;

//������������ ���������� ��������� ��� ����������� ��������� ��������� ����� �� ������� ����.
const char EMPTY = '_';
const char PIPE = 'O';
const char MISSED = '.';
const char WOUNDED = 'x';
const char KILLED = 'X';
const char UNKNOWN = '?';
const int BSIZE = 10; // ������������ ������ �������� ���� (10x10 �����).

//������������ ��� cell ��� ���� ������� (����� �������) � ������ ����� (����� ������) ��� ������������� ��������� ������.
typedef pair<char, int> cell;

const cell INVALID_CELL = cell{ 'X', 99 };

class Ship
{
public:
	int ship_size; //������ �������
	vector<cell> coord; //���������� ������, ���. �������
	set<cell> hits; //��������� ������, �� ������� ������� ��� ������
	bool killed = false; //����, �����������, ���� �� �������
	bool has_cell(cell); //����� ��� ��������, �������� �� ������� ��������� ������
	char hit(cell); //����� ��� ��������� �� �������
	char cell_state(cell); //����� ��� ����������� ��������� ������ �������.
	void killed_check();
	bool near(cell c);
	Ship(vector<cell> crd): //����������� ������ Ship
		ship_size(crd.size()),
		coord(crd)
	{}
};

class Board
{
	vector<Ship> ships; //������ �������� �� ������� ����
	set<cell> shoots; //��������� ������, �� ������� ���� ����������� ���������
	bool expl = false; //����, �����������, ��������� �� �����
	bool display_pipes = true;
public:
	Board(bool is_human_board = true)
	{
		display_pipes = is_human_board;
	}
	static cell ij_to_cell(int i, int j) //����������� ����� ��� �������������� �������� � ������
	{
		cell c{ 'A' + j, 1 + i };
		return c;
	}
	bool put_ship(Ship ship) //����� ��� ���������� ������� �� ������� ����
	{
		ships.push_back(ship);
		return true;
	}
	bool ship_nearby(cell c);
	void display_board();
	bool all_ships_killed();
	char shoot(cell target);
	char cell_state(cell);
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
	cell generate_shoot();
	cell get_shot_coord();
	bool auto_place_ships(int variant = 0);
	bool manual_place_ships();
};

bool string_to_cell(string in, cell& c);
void intro();
bool is_cell_valid(cell c);
bool get_coords(vector<cell>& cells, int size);
bool is_cells_near(cell c1, cell c2);
bool check_coords(vector<cell>& cells, Board& board);