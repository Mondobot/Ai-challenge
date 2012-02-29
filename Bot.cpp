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
		
		//makeMoves();
		setAnts();
		assignFood();
		
		writeMoves();
		endTurn();
		LOG("Time taken: " << state.timer.getTime() << "ms" << std::endl);
	}
}

char Bot::okMove(Location dest , Location src)
{
	if (all_moves.find(dest) != all_moves.end())
		return 0;

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
							all_moves.insert(std::make_pair(newLoc , mv));
							
							state.grid[newLoc2.row][newLoc2.column].assign = 1;
							d = 4; dir = 4; while (!q.empty()) q.pop();
							q.push(newLoc);
							vizfood[state.grid[newLoc.row][newLoc.column].vizitat] = 1;
							vizant[state.grid[newLoc2.row][newLoc2.column].antNr] = 1;
							state.grid[newLoc2.row][newLoc2.column].antPlayer = -1;
							state.grid[newLoc2.row][newLoc2.column].assign = 0;
							state.grid[newLoc.row][newLoc.column].antPlayer = 0;
							state.grid[newLoc.row][newLoc.column].assign = 1;
							state.grid[newLoc.row][newLoc.column].antNr 
								= state.grid[newLoc2.row][newLoc2.column].antNr;
							state.grid[newLoc2.row][newLoc2.column].antNr = -1;
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


}

void relative_pos(Location x, Location y, int mask[4])
{
	mask[NORTH] = mask[EAST] = mask[WEST] = mask[SOUTH] = 0;
	int poz[2];
	int dx = x.row - y.row;
	int dxx = dx < 0 ? -dx : dx;
	int dy = x.column - y.column;
	int dyy = dy < 0 ? -dy : dy;

	int dr = dxx < (gparam::mapRows - dxx) ? dxx : (gparam::mapRows - dxx);
	int dc = dyy < (gparam::mapColumns - dyy) ? dyy : (gparam::mapColumns - dyy);

	/* Calculationg the relative position properly*/
	poz[0] = (dx > 0) ? -1 : 1;
	poz[1] = (dy > 0) ? 1 : -1;

	if (dr != dxx)
		poz[0] *= -1;
	if (dc != dyy)
		poz[1] *= -1;
	if (dr > dc)
		poz[1] = 0;
	if (dc > dr)
		poz[0] = 0;

	/* Fill the mask*/
	if (poz[1] == 1)
		mask[WEST] = 1;

	if (poz[1] == -1)
		mask[EAST] = 1;

	if (poz[0] == 1)
		mask[SOUTH] = 1;
	
	if (poz[0] == -1)
		mask[NORTH] = 1;
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

	//std::cout << state.currentTurnNumber - 1;
	std::cout << "go" << std::endl;
}

