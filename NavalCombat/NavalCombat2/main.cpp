#include "naval.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    Board human_board;
    Board computer_board(false);
    AutoGamer autogamer_human(human_board);
    AutoGamer autogamer_computer(computer_board);
    srand(static_cast<unsigned int>(time(0)));

    cell first_shoot_hint = cell('D', 6);

    intro();
    pause();
    system("cls");

    cout << "Разместите корабли: " << endl;
    if (!autogamer_human.auto_place_ships(1))
    {
        exit(0);
    }
   human_board.display_board();

    autogamer_computer.auto_place_ships(0);
   computer_board.display_board();

    bool human_turn = true;
    while (!human_board.all_ships_killed() && !computer_board.all_ships_killed())
    {
        system("cls");
        human_board.display_board();
        computer_board.display_board();
        if (human_turn)
        {
            cell target = autogamer_human.get_shot_coord();

            char result = computer_board.shoot(target);
            cout << "Выстрел по: " << target.first << target.second << " Результат: " << result << endl;
            if (result == MISSED)
            {
                human_turn = false;
            }
        }
        else
        {
            cell target = autogamer_computer.generate_shoot();

            // temp code begin        
            if (first_shoot_hint != INVALID_CELL)
            {
                target = first_shoot_hint;
                first_shoot_hint = INVALID_CELL;
            }
            // temp code end

            char result = human_board.shoot(target);
            autogamer_computer.delete_possible_shoots(target);
            autogamer_computer.update_hunting_mode(result, target);
            cout << "Компьютер выстрелил по: " << target.first << target.second << " Результат: " << result << endl;          
            if (result == MISSED)
            {
                human_turn = true;
            }
            if (result == WOUNDED || result == KILLED)
            {
                autogamer_computer.update_impossible_shoots(target);
                autogamer_computer.update_possible_shoots(target, autogamer_human);
                pause();
            }
        }
    }

    if (human_board.all_ships_killed())
    {
        cout << "Победил компьютер!" << endl;
    }
    cout << "Победил игрок!" << endl;
}