#include "naval.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    Board human_board;
    Board computer_board(false);
    AutoGamer autogamer_human(human_board);
    AutoGamer autogamer_computer(computer_board);

    intro();

    cout << "Разместите корабли: " << endl;
    autogamer_human.place_ships_human();
    human_board.display_board();

    autogamer_computer.place_ships_computer();
    computer_board.display_board();

    bool human_turn = true;
    while (!human_board.all_ships_killed() && !computer_board.all_ships_killed())
    {
        if (human_turn)
        {
            cell target = autogamer_human.get_shot_coord();
            char result = computer_board.shoot(target);
            cout << "Выстрел по: " << target.first << target.second << " Результат: " << result << endl;
            computer_board.display_board();
        }
        else
        {
            cell target = autogamer_computer.generate_shoot();
            char result = human_board.shoot(target);
            cout << "Компьютер выстрелил по: " << target.first << target.second << " Результат: " << result << endl;
            human_board.display_board();
        }
        human_turn = false;
    }

    if (human_board.all_ships_killed())
    {
        cout << "Победил компьютер!" << endl;
    }
    cout << "Победил игрок!" << endl;
}