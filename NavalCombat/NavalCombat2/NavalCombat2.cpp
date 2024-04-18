#include "naval.h"

int main()
{
    Board board;
    Ship ship(1, { {'A', 1}});
    Ship ship1(4, { {'C', 5},  {'C', 6},  {'C', 7},  {'C', 8} });
    board.put_ship(ship);
    board.put_ship(ship1);
    board.dispay_board();
}