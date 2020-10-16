#pragma once
#include <SDL2/SDL.h>
#include <list>

#include "../headers/GameStateManager.h"
#include "../headers/Background.h"
#include "../headers/TileMap.h"
#include "../headers/Enemy.h"
#include "../headers/Explosion.h"
#include "../headers/AudioPlayer.h"
#include "../headers/BgObject.h"
#include "../headers/TriggerBox.h"
#include "../headers/Letters.h"
#include "../headers/Pig_Player.h"
#include "../headers/Cake.h"



#define NUM_OPTIONS 2
class GameStateManager;
class GameState
{
	protected:
		GameStateManager *gsm;
		SDL_Renderer *renderTarget;

	public:
		virtual void init() =0;
		virtual void update() =0;
		virtual void draw() =0;
		virtual void keyPressed(int k) =0;
		virtual void keyReleased(int k) =0;
};
class PigState : public GameState
{
    private:
        static constexpr int SFX_NUM = 3;
        TileMap *tileMap;
        Background *bg;
        Pig_Player *pig_player;
        Cake *cake;


        int pig_startX, pig_startY;
        int score;
        int lives;
        bool game_over;
        bool game_win;
        bool player_died;
        bool create_pressed;

        SDL_Texture *preScaleTexture;
        SDL_Texture *gameWorldTexture;


        bool trackPlayerY;
        int playerYLock;

        AudioPlayer *bgm;
        AudioPlayer **level_sounds;


        void reset();
        void updateGameCamera();
    public:
        PigState(GameStateManager *gsm_, SDL_Renderer *renderTarget_);
        ~PigState();
        void init();
        void update();
        void draw();
        void keyPressed(int k);
        void keyReleased(int k);
};
class LevelCreator : public GameState
{
    private:
        TileMap *tileMap;
        Background *bg;

        SDL_Texture *preScaleTexture;

        std::string map_name;
        int cursor_x, cursor_y;
        int tile_idx;
        bool space_pressed;
        bool save_pressed;
        bool play_pressed;

        void reset();

    public:
        LevelCreator(GameStateManager *gsm_, SDL_Renderer *renderTarget_);
        ~LevelCreator();
        void init();
        void update();
        void draw();
        void keyPressed(int k);
        void keyReleased(int k);
};
