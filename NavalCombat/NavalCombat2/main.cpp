#include "naval.h"

void draw(Board& h, Board& c, string msg)
{
	system("cls");
	h.display_board();
	c.display_board();
	cout << msg << endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	srand(static_cast<unsigned int>(time(nullptr)));
	Board human_board;
	Board computer_board(false);
	AutoGamer autogamer_human(human_board);
	AutoGamer autogamer_computer(computer_board);
	bool human_turn = true;

	intro();
	pause();

	autogamer_human.place_ships();

	autogamer_computer.auto_place_ships_random();
	draw(human_board, computer_board, "");

	while (!human_board.all_ships_killed() && !computer_board.all_ships_killed())
	{		
		stringstream message;
		if (human_turn)
		{
			cell target = autogamer_human.get_shot_coord();

			char result = computer_board.shoot(target);
			message << "Выстрел по: " << target.first << target.second << " Результат: " << result << endl;
			if (result == MISSED)
			{
				human_turn = false;
			}
		}
		else
		{
			cell target = autogamer_computer.generate_shoot(autogamer_human);
			char result = human_board.shoot(target);
			autogamer_computer.delete_possible_shoots(target);
			autogamer_computer.update_hunting_mode(result, target);
			message << "Компьютер выстрелил по: " << target.first << target.second << " Результат: " << result << endl;
			if (result == MISSED)
			{
				human_turn = true;
			}
			if (result == WOUNDED || result == KILLED)
			{
				autogamer_computer.update_impossible_shoots(target);
				autogamer_computer.update_possible_shoots(target, autogamer_human);
			}
			if (result == KILLED)
			{
				autogamer_computer.invalidate_poss();
			}
		}
		draw(human_board, computer_board, message.str());
		//autogamer_computer.imp_poss_draw();
		if (!human_turn) pause();
	}

	if (human_board.all_ships_killed())
	{
		cout << "Победил компьютер!" << endl;
	}
	else
	{
		cout << "Победил игрок!" << endl;
	}
	
}