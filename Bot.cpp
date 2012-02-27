#include "Bot.h"

void Bot::playGame()
{
  /* Reads in game parameters. */
  LOG("Reading initial parameters.");
  std::cin >> state;
  endTurn();
  LOG(gparam::spawnRadius);
  srand((unsigned int)gparam::seed);

  /* Continues to make moves until game is over. */
  while(std::cin >> state) {
    LOG("turn " << state.currentTurnNumber << ":");
    state.mark_visible();
	setAnts();
	assignFood();
	addMoves();

	writeMoves();
	
	endTurn();
	LOG("Time taken: " << state.timer.getTime() << "ms" << std::endl);
  }
}



char Bot::okMove(Location dest , Location src)
{
	if (state.grid[dest.row][dest.column].vizitat !=-1)
		return 0;
	
	if (state.distance (src , dest) < gparam::spawnRadius) 
		return 1;

	if (state.distance (src , dest) > 3 * gparam::viewRadius)
		return 0;

	if (state.grid[dest.row][dest.column].isHill)
		return 0;

	return (!state.grid[dest.row][dest.column].isWater &&
			state.grid[dest.row][dest.column].isVisible &&
		        	state.grid[dest.row][dest.column].antPlayer == -1);
}

void Bot::setAnts()
{
	for (int ant = 0; ant < (int)state.myAnts.size(); ++ant)
		state.grid[state.myAnts[ant].row][state.myAnts[ant].column].antNr = ant;
}

void Bot::addMoves()
{
	Location loc;
	Move mv;
	for (int it = 0; it < (int)assignments.size(); ++it) 
	{	
		loc = assignments[it].first;
		mv = assignments[it].second;		
		all_moves.insert(std::make_pair(loc , mv));
	}
}

void Bot::resetVec(std::vector<int> *vec , int size)
{
	(*vec).clear();
	for (int i = 0; i < size; i++)
		(*vec).push_back(0);
}

int Bot::zero(std::vector<int> vec)
{
	for (int i = 0; i < (int)vec.size(); i++)
		if (vec[i] == 0)
			return 0;
	return 1;
}

void Bot::assignFood()
{
	std::vector<int> vizfood;
	std::vector<int> vizant;
	Location newLoc , newLoc2;
	Move mv;
	int exit = 1;
	resetVec(&vizfood , state.food.size());
	resetVec(&vizant , state.myAnts.size());
	
	while (!zero(vizfood) && !zero(vizant) && exit)
	{	
		
		std::queue <Location> q;
		std::queue <Location> viz;
		
			
		for (int i = 0; i < (int)vizfood.size(); i++)	
		{
			
			if (!vizfood[i])
			{
				
				q.push(state.food[i]);
				viz.push(state.food[i]);
				state.grid[state.food[i].row][state.food[i].column].vizitat = i;
			}
		}
		exit = 0;
		
		while (!q.empty())
		{
				
			
			for (int dir = 0; dir < 4; ++dir)
			{
				newLoc = (q.front()).move(dir);				
				
				if (okMove(newLoc , state.food[state.grid[(q.front()).row][(q.front()).column].vizitat]))
				{
					
					state.grid[newLoc.row][newLoc.column].vizitat = state.grid[(q.front()).row][(q.front()).column].vizitat;
					q.push(newLoc);
					viz.push(newLoc);
					for (int d = 0; d < 4; d++)
					{
						newLoc2 = newLoc.move(d);
						if (state.grid[newLoc2.row][newLoc2.column].antPlayer == 0 &&
							state.grid[newLoc2.row][newLoc2.column].assign == 0)
						{

							
							mv.loc = newLoc2;
							mv.dir = (d + 2) % 4;
							assignments.push_back(std::make_pair(newLoc , mv));
							
							state.grid[newLoc2.row][newLoc2.column].assign = 1;
							d = 4; dir = 4; while (!q.empty()) q.pop();
							q.push(newLoc);
							vizfood[state.grid[newLoc.row][newLoc.column].vizitat] = 1;
							
							vizant[state.grid[newLoc2.row][newLoc2.column].antNr] = 1;
							exit = 1;
						}
					}
				}

			}
			q.pop();
		}

		while (!viz.empty())
		{
			newLoc = viz.front();
			state.grid[newLoc.row][newLoc.column].vizitat = -1;
			viz.pop();
		}
		
	}


	/*for (int f = 0; f < (int)state.food.size(); ++f) {
		std::queue <Location> q;
		std::queue <Location> viz;
		state.grid[state.food[f].row][state.food[f].column].vizitat = 1;
		q.push(state.food[f]);
		viz.push(state.food[f]);
		while (!q.empty())
		{
			for (int dir = 0; dir < 4; ++dir)
			{
				newLoc = (q.front()).move(dir);				
				
				if (okMove(newLoc , state.food[f]))
				{
					state.grid[newLoc.row][newLoc.column].vizitat = 1;
					q.push(newLoc);
					viz.push(newLoc);
					for (int d = 0; d < 4; d++)
					{
						//d = dir;
						newLoc2 = newLoc.move(d);
						if (state.grid[newLoc2.row][newLoc2.column].antPlayer == 0 &&
							state.grid[newLoc2.row][newLoc2.column].assign == 0)
						{
							mv.loc = newLoc2;
							mv.dir = (d + 2) % 4;
							assignments.push_back(std::make_pair(newLoc , mv));
							state.grid[newLoc2.row][newLoc2.column].assign = 1;
							d = 4; dir = 4; while (!q.empty()) q.pop();
							q.push(newLoc);
						}
					}
				}

			}
			q.pop();
		}

		while (!viz.empty())
		{
			newLoc = viz.front();
			state.grid[newLoc.row][newLoc.column].vizitat = 0;
			viz.pop();
		}
		
	}*/
}


void Bot::makeMoves()
{
/*	std::pair<Location, Move> new_move;
	for (int ant = 0; ant < (int)state.myAnts.size(); ++ant) {
		int direction = rand() % 4;
		Location newLoc = state.myAnts[ant].move(direction);
		
		if (validMove(newLoc)) {
			new_move.first.row = newLoc.row;
			new_move.first.column = newLoc.column; 
			new_move.second.loc.row = state.myAnts[ant].row;
			new_move.second.loc.column = state.myAnts[ant].column;
			new_move.second.dir = direction;
			
			all_moves.insert(new_move);*/

			/*
			if (all_moves.find(new_move.first) != all_moves.end()) {
				std::cout << "*************\n";
				std::cout << "*ahahahahaha*\n";
				std::cout << "*************\n\n";
			}
			state.grid[newLoc.row][newLoc.column].antPlayer = 0;
			state.grid[state.myAnts[ant].row][state.myAnts[ant].column].antPlayer = -1;
			*/
/*		}
	}*/
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
		LOG("o" << " " << m.loc.row);
		LOG(" " << m.loc.column << " ");
		LOG(DIRECTION_LETTER[m.dir]);
	}
  
}

void Bot::endTurn()
{
	all_moves.clear();
	assignments.clear();
  LOG("Sending endTurn()");

  /* If this wasn't the start game, reset the board. */
  if(state.currentTurnNumber > 0) {
      state.reset();
  }

  /* Move to next turn. */
  state.currentTurnNumber++;

  std::cout << "go" << std::endl;
}

