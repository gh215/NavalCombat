#include "naval.h"


void intro()
{
	cout << "\t����� ���������� � ���� \"������� ���\"!" << endl;
	cout << "���� ������ - ���������� ��� ������� ����������." << endl;
	cout << "������� ����������� �� ������� ���� 10x10." << endl;
	cout << "������� ����� ���� ��������� ��������: 1, 2, 3, 4 ������." << endl;
	cout << "���� ���������� � ���������� ��������." << endl;
	cout << "��� ������ ���������� � �������� �� �����������." << endl;
	cout << "���� ����� � ������� ��� ����, �� ��� �� ���������." << endl;
	cout << "���� ������������ �� ��� ���, ���� ���� �� ������� �� �������� ��� ������� ����������." << endl;
	cout << "�����!" << endl;
}

bool is_cell_valid(cell c)
{
	if ((c.first >= 'A' && c.first <= 'J') && (c.second >= 1 && c.second <= 10))
	{
		return true;
	}
	return false;
}

bool string_to_cell(string in, cell& c)
{
	if (in.size() != 2 && in.size() != 3) return false;

	char col = toupper(in[0]);
	if (col < 'A' || col > 'J') return false;
	int row = stoi(in.substr(1));
	if (row < 1 || row > 10) return false;

	c = cell(col, row);
	return true;
}

bool get_coords(vector<cell>& cells, int size)
{
	string input;
	cout << "������� ���������� ������� �������: " << size << "(��������, A1 B1 C1)" << endl;
	getline(cin, input);
	istringstream str(input);
	string word;

	while (str >> word)
	{
		cell c;
		if (!string_to_cell(word, c))
		{
			return false;
		}
		cells.push_back(c);
	}
	if (cells.size() != size)
	{
		return false;
	}
	return true;
}

bool is_cells_near(cell c1, cell c2)
{
	if (abs(c1.first - c2.first) > 1 || abs(c1.second - c2.second) > 1)
	{
		return false;
	}
	if (c1.first == c2.first && c1.second == c2.second)
	{
		return false;
	}
	return true;
}

bool check_coords(vector<cell>& cells, Board& board)
{
	// ���������, ��� ���������� �� ������������ � ��� ������������ ���������
	for (auto c : cells)
	{
		if (board.cell_state(c) != EMPTY)
		{
			cout << "���������� ������������ � ��� ����������� ��������." << endl;
			return false;
		}
		if (board.ship_nearby(c))
		{
			cout << "������� �� ����� ���� �������� ����� � ������ �������." << endl;
			return false;
		}
	}
	return true;
}

