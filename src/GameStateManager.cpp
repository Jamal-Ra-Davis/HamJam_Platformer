#include <stdio.h>
#include "../headers/GameStateManager.h"


GameStateManager::GameStateManager(SDL_Renderer *renderTarget_)
{
	renderTarget = renderTarget_;

	currentState = PIG_STATE;

	score = 0;
	gameStates = new GameState*[NUM_STATES];
	for (int i=0; i<NUM_STATES; i++)
	{
		gameStates[i] = NULL;
	}
	loadState(currentState);
	gameStates[currentState]->init();
	printf("Loaded title state\n");
}

GameStateManager::~GameStateManager()
{
	for (int i=0; i<NUM_STATES; i++)
    {
        if (gameStates[i])
			delete gameStates[i];
    }
	if (gameStates)
		delete[] gameStates;
}

void GameStateManager::setState(int state)
{
	unloadState(currentState);
	currentState = state;
	loadState(currentState);
	gameStates[currentState]->init();
}
void GameStateManager::update()
{
	gameStates[currentState]->update();
}
void GameStateManager::draw()
{
	gameStates[currentState]->draw();
}
void GameStateManager::keyPressed(int k)
{
	gameStates[currentState]->keyPressed(k);
}
void GameStateManager::keyReleased(int k)
{
	gameStates[currentState]->keyReleased(k);
}

void GameStateManager::loadState(int state)
{
    if (state >= NUM_STATES || state < 0)
        return;
    if (gameStates[state])
        return;

	switch (state)
	{
        case PIG_STATE:
        {
            gameStates[state] =  new PigState(this, renderTarget);
            break; 
        }
        case LEVEL_CREATOR_STATE:
        {
            gameStates[state] =  new LevelCreator(this, renderTarget);
            break;
        }
        default:
        {

        }
	}
}
void GameStateManager::unloadState(int state)
{
	if (gameStates[state])
	{
		delete gameStates[state];
		gameStates[state] = NULL;
	}
}
