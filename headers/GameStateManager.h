#pragma once
#include <SDL2/SDL.h>

#include "../headers/GameState.h"


class GameState;
class GameStateManager
{
	private:
		GameState **gameStates;
		int currentState;
		SDL_Renderer *renderTarget;
		int score;

		void loadState(int state);
		void unloadState(int state);
	public:
		enum GameStateNames
		{
    		PIG_STATE, LEVEL_CREATOR_STATE, NUM_STATES
		};

		GameStateManager(SDL_Renderer *renderTarget_);
		~GameStateManager();
		int getScore() {return score;}
		void setScore(int s) {score = s;}

		void setState(int state);
		void update();
		void draw();

		void keyPressed(int k);
		void keyReleased(int k);
};
