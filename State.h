/** Structure for storing information abour the current state of the map. */
#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstdlib>
#include <stdint.h>
#include <map>
#include <utility>

#include "global.h"

#include "Logging.h"
#include "Timer.h"

/** Structure for representing a location on the map. */
struct Location {
  int row;
  int column;
  bool invalid;
	
	Location () {invalid = 0;}

  Location(int row, int column) : row(row), column(column) { invalid = 0; }

  Location move(int dir) {
    Location returnValue(row + ROW_DIRECTION[dir],
                         column + COLUMN_DIRECTION[dir]);
    
    if (returnValue.row < 0) {
      returnValue.row += gparam::mapRows;
    } else if (returnValue.row == gparam::mapRows) {
      returnValue.row = 0;
    }

    if (returnValue.column < 0) {
      returnValue.column += gparam::mapColumns;
    } else if (returnValue.column == gparam::mapColumns) {
      returnValue.column = 0;
    }

    return returnValue;
  }
};

/** Structure that contains an ant move*/
struct Move {
	Location loc;
	int dir;

	Move () {}

	Move (Location loc , int dir) {
		this->loc = loc;
		this->dir = dir;
	}
};

/** Functie de comparat locatii */
struct compare_loc {
	bool operator()(Location x, Location y) {
		
		return (x.row > y.row) || ((x.row == y.row) && (x.column > y.column));
	}
};

/** Struct for representing a square in the grid. */
struct Square
{
    bool isVisible;
    bool isWater;
    bool isHill;
    bool isFood;
    int hillPlayer;
    int antPlayer;
    int vizitat;
    bool assign;
    int antNr;
    

    Square() : isVisible(false), isWater(false), isHill(false), isFood(false)
    {
      hillPlayer = antPlayer = -1;
	vizitat = -1;
	assign = 0;
    }

    /** Resets the information for the square except water information. */
    void reset()
    {
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
  State() : gameOver(false), currentTurnNumber(0)
  {
    for (int i = 0; i < MAXIMUM_MAP_SIZE; ++i) {
      grid.push_back(std::vector<Square>(MAXIMUM_MAP_SIZE, Square()));
    }
  }

  /** Clears non-persistent informatin from the grid after a step. */
  void reset();

  /** Marks visible cells. */
  void mark_visible();

  /** This is just square of Euclid distance. */
  double distance(const Location loc1, const Location loc2);

};




/** Method that helps do the IO. */
std::istream& operator>>(std::istream &is, State &state);

#endif

