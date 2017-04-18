#include "board_betting.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>

const int START_CREDITS = 10;

MoveBetting::MoveBetting(Token player,int bet) : Move(player), bet(bet) {}

void MoveBetting::print() const {
	if (player!= NOT_PLAYED) std::cout<<"bet " << this->bet <<" for player "<<player;
	else std::cout<<"null bet move";
}

Move *MoveBetting::deepcopy() const {
	Move *copy=new MoveBetting(player,bet);
	return copy;
}

bool MoveBetting::compare (const Move& abstract_move) const {
    const MoveBetting &move=dynamic_cast<const MoveBetting&>(abstract_move);
	return Move::compare(abstract_move) and move.bet == bet;
}

BoardBetting::BoardBetting() : played_count(0),
  credits({{START_CREDITS,START_CREDITS}}),
  bet_p1(0),
  bet_winner(NOT_PLAYED)
{
  for (int i=0; i<3; ++i) {
    board[i].fill(NOT_PLAYED);
  }
}

BoardBetting::~BoardBetting() {
}

Board *BoardBetting::deepcopy() const {
    BoardBetting *copy=new BoardBetting();
    //copy last move and played_count
    copy->board=board;
    copy->played_count = played_count;
    copy->credits = credits;
    copy->bet_p1 = bet_p1;
    copy->bet_winner = bet_winner;
    return copy;
}

Move *BoardBetting::parse_move_string(Token player,const char *string) const {
	std::stringstream stream(std::stringstream::in | std::stringstream::out);
  int movetype;
	int row=-1, column=-1;

	stream<<string;
  movetype = stream.get();

	if (stream.fail()) return NULL;

  Move* move = nullptr;
  if (movetype == 'p') {
    stream>>row>>column;
    std::cout << movetype << ":" << column << ":" << row << std::endl;
    move=new MoveTTT(player,row, column);
  } else if (movetype == 'b') {
    int amount;
    stream>>amount;
    std::cout << movetype << ":" << amount << std::endl;
    move=new MoveBetting(player,amount);
  } else {
    throw "Invalid move string";
  }

	if (is_move_valid(*move)) return move;

	delete move;
	return NULL;
}

void BoardBetting::print() const {
  for(int i=0; i<3; ++i) {
    for (int j=0; j<3; ++j) {
      std::cout << board[i][j];
    }
    std::cout << std::endl;
  }
  std::cout << "Credits: " << credits[0] << "/" << credits[1] << std::endl;
  std::cout << "Played count: " << played_count << std::endl;
}

bool BoardBetting::is_move_valid(const Move &abstract_move) const {
  if (dynamic_cast<const MoveTTT*>(&abstract_move)) {
    return is_move_valid(dynamic_cast<const MoveTTT&>(abstract_move));
  } else if (dynamic_cast<const MoveBetting*>(&abstract_move)) {
    return is_move_valid(dynamic_cast<const MoveBetting&>(abstract_move));
  } else {
    throw "Unsupported move type";
  }
}

bool BoardBetting::is_move_valid(const MoveTTT &move) const {
	return move.player!=NOT_PLAYED and move.column>=0 and move.column<3
    and move.row>=0 and move.row < 3
    and board[move.row][move.column] == NOT_PLAYED
    and played_count % 4 >= 2; // TTT move
}

bool BoardBetting::is_move_valid(const MoveBetting &move) const {
	return move.player!=NOT_PLAYED
    and move.bet >= 0
    and move.bet <= credits[move.player-1]
    and played_count % 4 < 2; // TTT move
}

Moves BoardBetting::get_possible_moves(Token player) const {
	Moves moves;
  if (played_count%4 >=2) {
    for(int i=0; i<3; ++i) {
      for (int j=0; j<3; ++j) {
        if (board[i][j] == NOT_PLAYED) {
          moves.push_back(new MoveTTT(player,i,j));
        }
      }
    }
  } else {
    for(int i=0; i<= credits[player-1]; ++i) {
      moves.push_back(new MoveBetting(player, i));
    }
  }
	return moves;
}

void BoardBetting::play_move(const Move &abstract_move) {
  if (played_count%4 >= 2) {
    const MoveTTT &move=dynamic_cast<const MoveTTT&>(abstract_move);
    assert(this->is_move_valid(move));
    if (bet_winner == move.player) {
      board[move.row][move.column] = move.player;
    }
  } else {
    const MoveBetting &move=dynamic_cast<const MoveBetting&>(abstract_move);
    assert(this->is_move_valid(move));
    if (played_count%4 == 0) {
      bet_p1 = move.bet;
    } else {
      credits[PLAYER_1-1] += (move.bet - bet_p1);
      credits[PLAYER_2-1] += (bet_p1 - move.bet);
      bet_winner = bet_p1 >= move.bet ? PLAYER_1 : PLAYER_2; // FIXME implement real rule
    }
  }
	played_count++;
}

bool BoardBetting::play_random_move(Token player) {
	if (played_count<9) {
		Moves possible_moves=get_possible_moves(player);
    assert(not possible_moves.empty());

		int selected=rand()/(RAND_MAX + 1.0) * possible_moves.size();
		Moves::const_iterator selected_iter=possible_moves.begin();
		while (selected>0) {
			selected--;
			selected_iter++;
		}
		play_move(**selected_iter);

		for (auto *i: possible_moves) delete i;

		return true;
	} else {
		//std::cout<<"board full"<<std::endl;
		return false;
	}
}

Token BoardBetting::check_for_win() const {
  // rows
  for (int row=0; row<3; ++row) {
    if (board[row][0] == board[row][1] and board[row][0] == board[row][2])
      return board[row][0];
  }
  // columns
  for (int column=0; column<3; ++column) {
    if (board[0][column] == board[1][column] and board[0][column] == board[2][column])
      return board[0][column];
  }
  // diagonals
  if (board[0][0] == board[1][1] and board[0][0] == board[2][2])
    return board[0][0];
  if (board[0][2] == board[1][1] and board[0][2] == board[2][0])
    return board[0][2];
  return NOT_PLAYED;
}

