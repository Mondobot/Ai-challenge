/** Structure for storing information about the current state of the map. */
#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdint.h>
#include <map>
#include <queue>
#include <utility>
#include <math.h>

#include "global.h"
#include "Logging.h"
#include "Timer.h"

/** Structure for representing a location on the map. */
struct Location {
	int row;
	int column;

	Location () {}

	Location(int row, int column) : row(row), column(column) {
		this->fix_this();
	}

	Location move(int dir) {
		Location returnValue(row + ROW_DIRECTION[dir],
                         column + COLUMN_DIRECTION[dir]);
		returnValue.fix_this();

		return returnValue;
	}

	/** Helper function for dealing with row/col < 0 || row/col > mapsize*/
	void fix_this() {
		if (this->row < 0) {
			this->row += gparam::mapRows;

		} else if (this->row == gparam::mapRows) {
			this->row = 0;
		}

		if (this->column < 0) {
			this->column += gparam::mapColumns;
		
		} else if (this->column == gparam::mapColumns) {
			this->column = 0;
		
		}
	}
};

/** Class that contains an ant move*/
struct Move {
	Location loc;
	int dir;

	Move () {}

	Move (Location loc , int dir) : loc(loc.row, loc.column) {
		this->dir = dir;
	}
};

/** Function for comparing locations */
struct compare_loc {
	bool operator()(Location x, Location y) {

		return (x.row > y.row) || ((x.row == y.row) && (x.column > y.column));
	}
};

/** Structure for representing a square in the grid. */
struct Square
{
	bool isVisible;
	bool isWater;
	bool isHill;
	bool isFood;
	int exploreVal;
	int hillPlayer;
	int antPlayer;
	int vizitat;
	bool assign;
	int antNr;

	Square() : isVisible(false), isWater(false), isHill(false), isFood(false) {
		hillPlayer = antPlayer = -1;
		exploreVal = 1;
		vizitat = -1;
		assign = 0;
	}

	/** Resets the information for the square except water information. */
	void reset() {
		isVisible = isHill = isFood = false;
		hillPlayer = antPlayer = -1;
		vizitat = -1;
		assign = 0;
	}
};

struct State
{
	/** False while we keep playing. */
	bool gameOver;

	int currentTurnNumber;

	/** Score for each of the current players. */
	std::vector<double> scores;

	/** See definition of Square for further details. */
	std::vector<std::vector<Square> > grid;

	std::vector<Location> myAnts;
	std::vector<Location> enemyAnts;
	std::vector<Location> myHills;
	std::vector<Location> enemyHills;
	std::vector<Location> food;

	/** This could have been global, but there you go... */
	Timer timer;

	/** Constructor creates the map proper. */
	State() : gameOver(false), currentTurnNumber(0) {

		for (int i = 0; i < MAXIMUM_MAP_SIZE; ++i) {
			grid.push_back(std::vector<Square>(MAXIMUM_MAP_SIZE, Square()));
		}
	}

	/** Clears non-persistent information from the grid after a step. */
	void reset();

	/** Marks visible cells. */
	void mark_visible();

private:
	void mark_visible_by_ant(Location ant);

public:
	/** This is just square of Euclid distance. */
	bool in_range(const Location loc1, Location loc2, double range);

	double distance(const Location loc1, const Location loc2);

};

/** Method that helps do the IO. */
std::istream& operator>>(std::istream &is, State &state);

#endif
