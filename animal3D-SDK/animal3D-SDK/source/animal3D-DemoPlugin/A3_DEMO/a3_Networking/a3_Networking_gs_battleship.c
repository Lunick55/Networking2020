/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein

	a3_Networking_gs_battleship.c/.cpp
	Game code for networked Battleship.

	********************************************
	*** Battleship implementation			 ***
	********************************************
*/

#include "a3_Networking_gs_battleship.h"

//-----------------------------------------------------------------------------
// DECLARATIONS


gs_battleship_space_state gs_checkers_getSpaceState(gs_battleship const game, gs_battleship_index const player, gs_battleship_index const xpos, gs_battleship_index const ypos)
{
	if (GS_BATTLESHIP_VALID(player, xpos, ypos))
		return (game[player][xpos][ypos]);
	return gs_battleship_space_invalid;
}

gs_battleship_space_state gs_checkers_setSpaceState(gs_battleship game, gs_battleship_space_state const state, gs_battleship_index const player, gs_battleship_index const xpos, gs_battleship_index const ypos)
{
	if (GS_BATTLESHIP_VALID(player, xpos, ypos))
		return (game[player][xpos][ypos] = state);
	return gs_battleship_space_invalid;
}

gs_battleship_index gs_battleship_reset(gs_battleship game)
{
	gs_battleship_index player, xpos, ypos, total;
	for (player = 0; player < GS_BATTLESHIP_PLAYERS; ++player)
		for (xpos = 0; xpos < GS_BATTLESHIP_BOARD_WIDTH; ++xpos)
			for (ypos = 0; ypos < GS_BATTLESHIP_BOARD_HEIGHT; ++ypos)
				game[player][xpos][ypos] = gs_battleship_space_open;
	total = (player * xpos * ypos);
	return total;
}


//-----------------------------------------------------------------------------
// DEFINITIONS

int launchBattleship()
{
	gs_battleship game = { 0 };

	gs_battleship_reset(game);



	return 0;
}


//-----------------------------------------------------------------------------
