#include "board_ttt.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>

MoveTTT::MoveTTT(Token player,Size row, Size column) : Move(player), row(row), column(column) {}

void MoveTTT::print() const {
	if (player!=NOT_PLAYED) std::cout<<"row"<<this->row<<" column "<<this->column<<" for player "<<player;
	else std::cout<<"c4 null move";
}

Move *MoveTTT::deepcopy() const {
	Move *copy=new MoveTTT(player,row,column);
	return copy;
}

bool MoveTTT::compare (const Move& abstract_move) const {
    const MoveTTT &move=dynamic_cast<const MoveTTT&>(abstract_move);
	return Move::compare(abstract_move) and column==move.column and row==move.row;
}

BoardTTT::BoardTTT() : played_count(0) {
  for (int i=0; i<3; ++i) {
    board[i].fill(NOT_PLAYED);
  }
}

BoardTTT::~BoardTTT() {
}

Board *BoardTTT::deepcopy() const {
    BoardTTT *copy=new BoardTTT();
    //copy last move and played_count
    copy->board=board;
    copy->played_count = played_count;
    return copy;
}

Move *BoardTTT::parse_move_string(Token player,const char *string) const {
	std::stringstream stream(std::stringstream::in | std::stringstream::out);
	int row=-1, column=-1;

	stream<<string;
	stream>>row>>column;

  std::cout << column << ":" << row << std::endl;

	if (stream.fail()) return NULL;

	Move *move=new MoveTTT(player,row, column);

	if (is_move_valid(*move)) return move;

	delete move;
	return NULL;
}

void BoardTTT::print() const {
  for(int i=0; i<3; ++i) {
    for (int j=0; j<3; ++j) {
      std::cout << board[i][j];
    }
    std::cout << std::endl;
  }
  std::cout << "Played count: " << played_count << std::endl;
}

bool BoardTTT::is_move_valid(const Move &abstract_move) const {
	return is_move_valid(dynamic_cast<const MoveTTT&>(abstract_move));
}

bool BoardTTT::is_move_valid(const MoveTTT &move) const {
	return move.player!=NOT_PLAYED and move.column>=0 and move.column<3
    and move.row>=0 and move.row < 3
    and board[move.row][move.column] == NOT_PLAYED;
}

Moves BoardTTT::get_possible_moves(Token player) const {
	Moves moves;
  for(int i=0; i<3; ++i) {
    for (int j=0; j<3; ++j) {
      if (board[i][j] == NOT_PLAYED) {
        moves.push_back(new MoveTTT(player,i,j));
      }
    }
  }
	return moves;
}

void BoardTTT::play_move(const Move &abstract_move) {
	const MoveTTT &move=dynamic_cast<const MoveTTT&>(abstract_move);
	assert(this->is_move_valid(move));
  board[move.row][move.column] = move.player;
	played_count++;
}

bool BoardTTT::play_random_move(Token player) {
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

		for (Moves::iterator iter=possible_moves.begin(); iter!=possible_moves.end(); iter++) delete *iter;

		return true;
	} else {
		//std::cout<<"board full"<<std::endl;
		return false;
	}
}

Token BoardTTT::check_for_win() const {
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
