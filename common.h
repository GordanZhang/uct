#ifndef __COMMON__
#define __COMMON__

#include <cassert>

#define MAX_INPUT_LENGTH 256

typedef int Size;
typedef enum {NOT_PLAYED=0,PLAYER_1=1,PLAYER_2=2} Token;

inline Token other_player(Token player) {
	switch (player) {
	case PLAYER_1:
		return PLAYER_2;
	case PLAYER_2:
		return PLAYER_1;
	default:
		throw "Unknown player";
	}
}

typedef enum {NORMAL,LOSER,WINNER} Mode;
typedef float Value;
typedef int Count;

#endif
