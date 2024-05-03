#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <set>

using namespace std;

//������������ ���������� ��������� ��� ����������� ��������� ��������� ����� �� ������� ����.
const char EMPTY = '_';
const char PIPE = 'O';
const char MISSED = '.';
const char WOUNDED = 'x';
const char KILLED = 'X';
const int BSIZE = 10; // ������������ ������ �������� ���� (10x10 �����).

//������������ ��� cell ��� ���� ������� (����� �������) � ������ ����� (����� ������) ��� ������������� ��������� ������.
typedef pair<char, int> cell;

void intro();

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
	void display_board();
	bool all_ships_killed();
	char shoot(cell target);
	char cell_state(cell);
};

class AutoGamer
{
public:
	Board& board; 
	AutoGamer(Board& board) : board(board) {} // ����������� ��������� ������ �� Board
	cell generate_shoot();
	cell get_shot_coord();
	void place_ships_computer();
	void place_ships_human();
};

