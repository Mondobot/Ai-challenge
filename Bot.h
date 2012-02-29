/** Structure that represents an AI in a game. */
#ifndef BOT_H_
#define BOT_H_

#include "State.h"



struct Bot
{
	/** Current state of the game. */
	State state;

	/** Vector for storring all moves*/
	std::map<Location, Move, compare_loc> all_moves;
	
	std::vector<std::pair<Location , Move> > assignments;	
	
	/** Plays a single game of Ants. */
	void playGame();

	/** Moves ants on the board. */
	void makeMoves();

	/* ceva */
	void addMoves();

 	/* Sends moves */
	void writeMoves();

	/* Helper functions for gathering food*/
	void resetVec(std::vector<int> * , int);
	int zero(std::vector<int>);
	char okMove(Location , Location);
	void assignFood();
	void setAnts();

	/** Checks for a valid move */
	bool validMove(Location);

  /** Indicates to the engine that it has made its moves. */
  void endTurn();
};

#endif

