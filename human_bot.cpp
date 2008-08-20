#include "uct.h"
#include "boardc4.h"
#include "player.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed,std::ios::floatfield);
    srand(time(NULL));

	Player *player_a=new PlayerHuman(PLAYER_1);
	Player *player_b=new PlayerBot(PLAYER_2);

	Player *player_current=player_a;
	Player *winner=NULL;

    Board *board=new BoardC4();

	while (true) {
		board->print();

		//get the move
		Move *move=player_current->get_move(board);
		if (not move) break;

		//actually play the move
		board->play_move(*move);
		delete move;

		//check for win
		if (board->check_for_win()) {
			winner=player_current;
			break;
		}

		//switch player
		if (player_current==player_a) player_current=player_b;
		else player_current=player_a;
	}

    board->print();
	if (winner) {
		std::cout<<"winner: ";
		winner->print();
	} else {
		std::cout<<"draw";
	}
	std::cout<<std::endl;

    delete board;
	delete player_a;
	delete player_b;

	return 0;
}
