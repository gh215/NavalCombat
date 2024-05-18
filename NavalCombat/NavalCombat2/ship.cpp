#include "naval.h"

bool Ship::has_cell(cell c) //����������� ������ has_cell ������ Ship, ������� ���������, �������� �� ������� ��������� ������
{
	auto res = find(coord.begin(), coord.end(), c); //���� ������ � ������� ��������� �������
	if (res == coord.end())
	{
		return false; //���� ������ �� �������, ���������� false
	}
	return true;
}

//��������� KILLED, ���� ������� ��� ����, WOUNDED, ���� ����� � EMPTY, ���� ������ ���
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
	if (hits.count(c) != 0) //���� ������ ������
	{
		return WOUNDED;
	}
	return PIPE;
}

void Ship::killed_check()
{
	if (hits.size() == ship_size) //���� ���������� ������, ������� �������� ������� ����� ���-�� ������, ���������� �������, �� �� ��������� ������������
	{
		killed = true;
	}
}

bool Ship::near(cell c)
{
	for (auto crd : coord)
	{
		if (is_cells_near(c, crd) && !has_cell(c))
		{
			return true;
		}
	}
	return false;
}