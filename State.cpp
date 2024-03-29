#include <cstdlib>
#include <iomanip>

#include "State.h"

/* Resets all non-water squares to land and clear the bots ant vector. */
void State::reset()
{
	myAnts.clear();
	myHills.clear();
	enemyAnts.clear();
	enemyHills.clear();
	food.clear();

	for(int row = 0; row < gparam::mapRows; row++)
		for(int col = 0; col < gparam::mapColumns; col++) {
			grid[row][col].exploreVal++;

			if(!grid[row][col].isWater)
				grid[row][col].reset();
		}
}

/* Returns the square of Euclid distance between two locations with the edges
 * wrapped. */
double State::distance(const Location loc1, const Location loc2)
{
	int d11 = loc1.row - loc2.row;
	int d1 = d11 < 0 ? -d11 : d11;
	int d22 = loc1.column - loc2.column;
	int d2 = d22 < 0 ? -d22 : d22;
	int dr = d1 < (gparam::mapRows - d1) ? d1 : (gparam::mapRows - d1);
	int dc = d2 < (gparam::mapColumns - d2) ? d2 : (gparam::mapColumns - d2);
	return dr*dr + dc*dc;
}

/** Simple wrapper to check if two locations are in range of each other*/
bool State::in_range(const Location loc1, const Location loc2, double range)
{
	return (distance(loc1, loc2) <= range);
}

/* Marks which squares on the map are visible by the given ant */
void State::mark_visible_by_ant(Location ant)
{
	Location start(ant.row - floor(sqrt(gparam::viewRadius)),
					ant.column - floor(sqrt(gparam::viewRadius)));
	Location stop(ant.row + floor(sqrt(gparam::viewRadius)),
					ant.column + floor(sqrt(gparam::viewRadius)));
	Location i = start;

	for (i.row = start.row; i.row != stop.row + 1; i = i.move(SOUTH))
		for (i.column = start.column; i.column != stop.column + 1; i = i.move(EAST)) {

			if (grid[i.row][i.column].isVisible)
				continue;

			/* Mark as visible and also reset the exploreVal of the square */
			if (in_range(i, ant, gparam::viewRadius)) {
				grid[i.row][i.column].isVisible = true;
				grid[i.row][i.column].exploreVal = 0;
			}

			 /*Debugging code:
			if (currentTurnNumber < 2)
				LOG(i.row << " " << i.column << " "<< grid[i.row][i.column].exploreVal);
			*/
		}
}

/** Marks all visible squares (actually it calls ^^ for each ant) */
void State::mark_visible()
{
	for (int i = 0; i < (int)myAnts.size(); ++i)
		mark_visible_by_ant(myAnts[i]);
}

/* Input functions. */
std::istream& operator>>(std::istream &is, State &state)
{
	int row, col, player;
	std::string inputType, junk;

	/* Read in input type. */
	while(is >> inputType) {
		if(inputType == "end") {
			state.gameOver = true;
			break;

		} else if(inputType == "turn") {
			is >> state.currentTurnNumber;
			break;

		} else {
			getline(is, junk);
		}
	}

	/* If we are at the beginning of the game, read in the parameters. */
	if(state.currentTurnNumber == 0) {
		while(is >> inputType) {
		    if(inputType == "loadtime") {
				is >> gparam::loadTime;

			} else if(inputType == "turntime") {
				is >> gparam::turnTime;

			} else if(inputType == "rows") {
				is >> gparam::mapRows;

			} else if(inputType == "cols") {
				is >> gparam::mapColumns;

			} else if(inputType == "turns") {
				is >> gparam::totalTurnsNumber;

			} else if(inputType == "player_seed") {
				is >> gparam::seed;
				srand((unsigned int)gparam::seed);

			} else if(inputType == "viewradius2") {
				is >> gparam::viewRadius;

			} else if(inputType == "attackradius2") {
				is >> gparam::attackRadius;

	 		} else if(inputType == "spawnradius2") {
				is >> gparam::spawnRadius;

			} else if(inputType == "ready") {
			/* This is the end of the parameter input. */
				state.timer.start();
				break;

			} else {
				getline(is, junk);
			}
		}

	/* Reads in information about the current turn. */
	} else {
		while(is >> inputType) {

			/* Water square. */
			if(inputType == "w") {
				is >> row >> col;
				state.grid[row][col].isWater = 1;

			/* Food square. */
			} else if(inputType == "f") {
				is >> row >> col;
				state.grid[row][col].isFood = 1;
				state.food.push_back(Location(row, col));

			/* Live ant square. */
			} else if(inputType == "a") {
				is >> row >> col >> player;
				state.grid[row][col].antPlayer = player;

				if(player == 0) {
					state.myAnts.push_back(Location(row, col));
				} else {
					state.enemyAnts.push_back(Location(row, col));
				}

			/* Dead ant squares. WARNING, still not reading anything*/
			} else if(inputType == "d") {
				is >> row >> col >> player;

			/* Hill square. */
			} else if(inputType == "h") {
				is >> row >> col >> player;
				state.grid[row][col].isHill = 1;
				state.grid[row][col].hillPlayer = player;

				if(player == 0) {
					state.myHills.push_back(Location(row, col));
				} else {
					state.enemyHills.push_back(Location(row, col));
				}

			/* Information about the players. */
			} else if(inputType == "players") {
				is >> gparam::numberPlayers;

			/* Information about the scores. */
			} else if(inputType == "scores") {
				state.scores = std::vector<double>(gparam::numberPlayers, 0.0);

				for(int p = 0; p < gparam::numberPlayers; p++) {
					is >> state.scores[p];
				}

			/* Finished input. */
			} else if(inputType == "go") {
				if(state.gameOver) {
					LOG("Received end of game message.");
					is.setstate(std::ios::failbit);

				} else {
					state.timer.start();
				}
				break;

			} else {
				getline(is, junk);
			}
		}
	}

	return is;
}
