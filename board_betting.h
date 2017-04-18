#ifndef __BOARDBetting__
#define __BOARDBetting__

#include <array>
#include "board.h"
#include "board_ttt.h"

class MoveBetting : public Move {
friend class BoardBetting;
public:
	MoveBetting(Token player,int bet);

  virtual void print() const;
	virtual Move *deepcopy() const;
	virtual bool compare (const Move& move) const;

protected:
  int bet;
};

class BoardBetting : public Board {
public:
	BoardBetting();
	virtual ~BoardBetting();

  virtual Board *deepcopy() const;
	virtual Move *parse_move_string(Token player,const char *string) const;
	virtual void print() const;
	inline virtual bool is_move_valid(const Move &move) const;
	inline bool is_move_valid(const MoveTTT &move) const;
  inline bool is_move_valid(const MoveBetting &move) const;
	virtual Moves get_possible_moves(Token player) const;
	virtual void play_move(const Move &move);
	virtual bool play_random_move(Token player);
	virtual Token check_for_win() const;

protected:
  bool board_full();
  std::array<std::array<Token, 3>, 3> board;
  int played_count;
  std::array<int, 2> credits;
  int bet_p1; // Player 1's bet, stored before bets are resolved
  Token bet_winner;
};

#endif
