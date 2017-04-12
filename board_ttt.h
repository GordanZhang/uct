#ifndef __BOARDTTT__
#define __BOARDTTT__

#include <array>
#include "board.h"

class MoveTTT : public Move {
friend class BoardTTT;
public:
	MoveTTT(Token player,Size row, Size column);

  virtual void print() const;
	virtual Move *deepcopy() const;
	virtual bool compare (const Move& move) const;

private:
    Size row;
    Size column;
};

class BoardTTT : public Board {
public:
	BoardTTT();
	virtual ~BoardTTT();

  virtual Board *deepcopy() const;
	virtual Move *parse_move_string(Token player,const char *string) const;
	virtual void print() const;
	inline virtual bool is_move_valid(const Move &move) const;
	inline bool is_move_valid(const MoveTTT &move) const;
	virtual Moves get_possible_moves(Token player) const;
	virtual void play_move(const Move &move);
	virtual bool play_random_move(Token player);
	virtual Token check_for_win() const;

private:
  std::array<std::array<Token, 3>, 3> board;
  int played_count;
};

#endif
