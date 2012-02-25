#include "Bot.h"

void Bot::playGame()
{
  /* Reads in game parameters. */
  LOG("Reading initial parameters.");
  std::cin >> state;
  endTurn();
  
  srand((unsigned int)gparam::seed);

  /* Continues to make moves until game is over. */
  while(std::cin >> state) {
    LOG("turn " << state.currentTurnNumber << ":");

    state.mark_visible();
	makeMoves();

	writeMoves();
	endTurn();
	LOG("Time taken: " << state.timer.getTime() << "ms" << std::endl);
  }
}

void Bot::makeMoves()
{
	std::pair<Location, Move> new_move;
	for (int ant = 0; ant < (int)state.myAnts.size(); ++ant) {
		int direction = rand() % 4;
		Location newLoc = state.myAnts[ant].move(direction);
		
		if (validMove(newLoc)) {
			new_move.first.row = newLoc.row;
			new_move.first.column = newLoc.column; 
			new_move.second.loc.row = state.myAnts[ant].row;
			new_move.second.loc.column = state.myAnts[ant].column;
			new_move.second.dir = direction;
			
			all_moves.insert(new_move);

			/*
			if (all_moves.find(new_move.first) != all_moves.end()) {
				std::cout << "*************\n";
				std::cout << "*ahahahahaha*\n";
				std::cout << "*************\n\n";
			}
			state.grid[newLoc.row][newLoc.column].antPlayer = 0;
			state.grid[state.myAnts[ant].row][state.myAnts[ant].column].antPlayer = -1;
			*/
		}
	}
}

bool Bot::validMove(Location l)
{
	return (!state.grid[l.row][l.column].isWater &&
        	state.grid[l.row][l.column].antPlayer == -1 &&
		all_moves.find(l) == all_moves.end());
}

void Bot::writeMoves()
{
	std::map<Location, Move>::iterator it;
	Move m;	

	for (it = all_moves.begin(); it != all_moves.end(); ++it) {
		m = (*it).second;
		std::cout << "o" << " " << m.loc.row;
		std::cout << " " << m.loc.column << " ";
		std::cout << DIRECTION_LETTER[m.dir] << std::endl; 
	}
  
}

void Bot::endTurn()
{
	all_moves.clear();
  LOG("Sending endTurn()");

  /* If this wasn't the start game, reset the board. */
  if(state.currentTurnNumber > 0) {
      state.reset();
  }

  /* Move to next turn. */
  state.currentTurnNumber++;

  std::cout << "go" << std::endl;
}

