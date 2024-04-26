#include "naval.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    Board human_board;
    Board computer_board;
    computer_board.is_human_board = false;
    AutoGamer autogamer_human(human_board);
    AutoGamer autogamer_computer(computer_board);


    Ship ship(1, { {'A', 1} });
    Ship ship1(4, { {'C', 5},  {'C', 6},  {'C', 7},  {'C', 8} });

    human_board.put_ship(ship);
    human_board.put_ship(ship1);

    autogamer_computer.place_ships();
    cell target = autogamer_computer.generate_shoot();

    char result = human_board.shoot(target);

    human_board.display_board();
    computer_board.display_board();

    cout << "\nРезультат выстрела: " << result << endl;
}