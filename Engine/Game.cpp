/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Board.h"
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	board(gfx),
	rng(std::random_device()()),
	snake({2, 2}),
	goal(rng, board, snake)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	if (!gameIsStarted)
	{
		gameIsStarted = wnd.kbd.KeyIsPressed(VK_RETURN);
		return;
	}
	if (gameIsOver)
		return;
	Location temp_loc;

	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		if (! ( (temp_loc = { 0, 1 }) == delta_loc ) ) {
			delta_loc = { 0, -1 };
		}
	}
	else if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		if (!((temp_loc = { 0, -1 }) == delta_loc)) {
			delta_loc = { 0, 1 };
		}
	}
	else if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		if (!((temp_loc = { 1, 0 }) == delta_loc)) {
			delta_loc = { -1, 0 };
		}
	}
	else if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		if (!((temp_loc = { -1, 0 }) == delta_loc)) {
			delta_loc = { 1, 0 };
		}
	}

	++snakeMoveCounter;
	if (snakeMoveCounter >= snakeMovePeriod)
	{
		snakeMoveCounter = 0;
		const Location next = snake.GetNextHeadLocation(delta_loc);
		if (!board.IsInsideBoard(next) || snake.IsInTileExceptEnd(next) ) {
			gameIsOver = true;
		}
		else {
			const bool eating = (next == goal.GetLocation());
			if (eating) {
				snake.Grow();
			}
			snake.MoveBy(delta_loc);
			if (eating) {
				goal.Respawn(rng, board, snake);
			}
		}
	}
	++snakeSpeedupCounter;
	if (snakeSpeedupCounter >= snakeSpeedupPeriod) {
		snakeSpeedupCounter = 0;
		snakeMovePeriod = std::max(snakeMovePeriod - 1, snakeMovePeriodMin);

	}
}

void Game::ComposeFrame()
{
	if (!gameIsStarted) {
		SpriteCodex::DrawTitle(290, 225, gfx);
		return;
	}

	snake.Draw(board);
	goal.Draw(board);
	if (gameIsOver) {
		SpriteCodex::DrawGameOver(350, 265, gfx);
		if (wnd.kbd.KeyIsPressed(VK_RETURN)) {
			snake = Snake({ 2,2 });
			goal = Goal(rng, board, snake);
			gameIsOver = false;
			delta_loc = { 1, 0 };
			snakeMovePeriod = 10; //initial: 10
			snakeMoveCounter = 0;
			snakeSpeedupCounter = 0;
		}
	}
	board.DrawBorder();
}