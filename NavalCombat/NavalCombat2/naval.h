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

//
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
	Ship(int size, vector<cell> crd): //����������� ������ Ship
		ship_size(size),
		coord(crd)
	{}
};

class Board
{
	vector<Ship> ships; //������ �������� �� ������� ����
	set<cell> shoots; //��������� ������, �� ������� ���� ����������� ���������
	bool expl = false; //����, �����������, ��������� �� �����
public:
	bool is_human_board = true;
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
	char shoot(cell target);
	char cell_state(cell);
};

class AutoGamer
{
public:
	Board& board; 
	AutoGamer(Board& board) : board(board) {} // ����������� ��������� ������ �� Board
	void place_ships();
	cell generate_shoot();
};
