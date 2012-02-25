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
	Move new_move;
	for (int ant = 0; ant < (int)state.myAnts.size(); ++ant) {
		int direction = rand() % 4;
		Location newLoc = state.myAnts[ant].move(direction);
		
		if (validMove(newLoc)) {
			new_move.loc.row = state.myAnts[ant].row;
			new_move.loc.column = state.myAnts[ant].column;
			new_move.dir = direction;
			
			all_moves.push_back(new_move);
			state.grid[newLoc.row][newLoc.column].antPlayer = 0;
			state.grid[state.myAnts[ant].row][state.myAnts[ant].column].antPlayer = -1;
		}
	}
}

bool Bot::validMove(Location newLocation)
{
	return (!state.grid[newLocation.row][newLocation.column].isWater &&
        state.grid[newLocation.row][newLocation.column].antPlayer == -1);
}

void Bot::writeMoves()
{
	for (int i = 0; i < (int)all_moves.size(); ++i) {
		std::cout << "o" << " " << all_moves[i].loc.row;
		std::cout << " " << all_moves[i].loc.column << " ";
		std::cout << DIRECTION_LETTER[all_moves[i].dir] << std::endl; 
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

