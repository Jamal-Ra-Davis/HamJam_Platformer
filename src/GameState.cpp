#include "../headers/GameState.h"
#include "../headers/SDL_Helpers.h"
#include "../headers/GamePanel.h"
#include "../headers/TileMap.h"
#include "../headers/Slugger.h"
#include "../headers/InputBuffer.h"

#define GAME_WINDOW_W GamePanel::WIDTH //480 //300
#define GAME_WINDOW_H GamePanel::HEIGHT //352 //220


PigState::PigState(GameStateManager *gsm_, SDL_Renderer *renderTarget_)
{
    gsm = gsm_;
    renderTarget = renderTarget_;

    tileMap = NULL;
    bg = NULL;
    pig_player = NULL;
    cake = NULL;

    preScaleTexture = NULL;
    gameWorldTexture = NULL;

    bgm = NULL;
    level_sounds = NULL;
}
PigState::~PigState()
{
    if (tileMap)
        delete tileMap;
    tileMap = NULL;

    if (bg)
        delete bg;
    bg = NULL;

    if (pig_player)
        delete pig_player;
    pig_player = NULL;

    if (cake)
        delete cake;
    cake = NULL;


    if (preScaleTexture)
        SDL_DestroyTexture(preScaleTexture);
    preScaleTexture = NULL;
    if (gameWorldTexture)
        SDL_DestroyTexture(gameWorldTexture);
    gameWorldTexture = NULL;



    if (bgm)
        delete bgm;
    if (level_sounds)
    {
        for (int i=0; i<SFX_NUM; i++)
        {
            if (level_sounds[i])
                delete level_sounds[i];
        }
        delete[] level_sounds;
    }
}
void PigState::init()
{

    pig_startX = 120;
    pig_startY = 160;

    score = 0;
    lives = 2;

    game_over = false;
    game_win = false;
    player_died = false;
    create_pressed = false;


    tileMap = new TileMap(30, renderTarget);
    //tileMap = new TileMap(12, renderTarget);
    tileMap->loadTiles("./Resources/Tilesets/grasstileset.bmp", "");
    //tileMap->loadTiles("./Resources/Tilesets/GoS_Tiles.bmp", "./Resources/Maps/GoS_TileTypes.txt");
    //tileMap->loadMap("./Resources/Maps/level1-1.map");
    tileMap->loadMap("./Resources/Maps/creator_map.map");
    tileMap->setPosition(0, 0);
    tileMap->setTween(1);

    bg = new Background("./Resources/Backgrounds/grassbg1.bmp", 0.1, renderTarget);
	if (bg == NULL)
	{
		printf("ERROR: Background didn't load\n");
		*(GamePanel::isRunningControl) = false;
	}
	

    pig_player = new Pig_Player(tileMap, renderTarget);
    pig_player->setPosition(pig_startX, pig_startY);
    pig_player->setVector(0, 0);
    pig_player->loadStats("./Resources/Files/Pig_Stats.txt");

    cake = new Cake(tileMap, renderTarget, 10);
    cake->setPosition(pig_startX + 150, pig_startY + 0);
    cake->setVector(0, 0);

    
    trackPlayerY = false;
    playerYLock = 105;

    bgm = new AudioPlayer("./Resources/Music/delightful-d.mp3", true);
    level_sounds = NULL;
    level_sounds = new AudioPlayer*[SFX_NUM];
    for (int i=0; i<SFX_NUM; i++)
    {
        level_sounds[i] = NULL;
    }
    //level_sounds[POOF_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_EnemyDies.wav", false);
    //level_sounds[LEVEL_START_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_LevelStart.wav", false);
    //level_sounds[LEVEL_LOSE_SFX] = new AudioPlayer(/*"./Resources/SFX/BMOGame_Death.wav"*/"./Resources/SFX/BMOGame_YouLose.wav", false);
    //level_sounds[RESPAWN_SFX] = new AudioPlayer("./Resources/SFX/BMOGame_Respawn.wav", false);


    //gameWorldTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GamePanel::WIDTH, GamePanel::HEIGHT);
    preScaleTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GAME_WINDOW_W, GAME_WINDOW_H);
    gameWorldTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GAME_WINDOW_W, 116);

    //Start level stuff
    bgm->play(AudioPlayer::LOOP);

}
void PigState::update()
{
    static int cnt = 0;
    //tileMap->setPosition(GamePanel::WIDTH/2 - pig_player->getX(),
    //                     GamePanel::HEIGHT/2 - pig_player->getY());
    //tileMap->setPosition(GamePanel::WIDTH/2 - pig_startX,
    //                     GamePanel::HEIGHT/2 - pig_startY);
    if (pig_player->getY() > tileMap->getHeight() + 100)
    {
        gsm->setState(GameStateManager::PIG_STATE);
        return;
    }
    if (create_pressed)
    {
       gsm->setState(GameStateManager::LEVEL_CREATOR_STATE);
       return; 
    }
    create_pressed = false;

    pig_player->update();
    cake->update();
    if (pig_player->intersects(cake) && !cake->isCollected())
    {
        cake->collect();
        pig_player->recharge();
    } 
    pig_player->recharge();


    tileMap->setPosition(GamePanel::WIDTH/2 - pig_player->getX(),
                         GamePanel::HEIGHT/2 - pig_player->getY());

    cnt++;
    if (cnt % 30 == 0)
    {
        int tile_idx = tileMap->getMapIdx(5, 5);
        tile_idx++;
        tile_idx %= 30;
        tileMap->setMapIdx(5, 5, tile_idx, false);
    }
}
void PigState::draw()
{

    SDL_DisplayMode current;
    SDL_GetDesktopDisplayMode(0, &current);
    int window_width, window_height;
    //SDL_GetWindowSize()
/*
    SDL_Texture *preScaleTexture = NULL;
    SDL_Texture *gameWorldTexture = NULL;

    //gameWorldTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GamePanel::WIDTH, GamePanel::HEIGHT);
    preScaleTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GAME_WINDOW_W, 152);
    gameWorldTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GAME_WINDOW_W, 116);
*/
    //SDL_Rect scaleRect = {11, 11+110, 3*GAME_WINDOW_W, 3*116};


    //Setup to render to gameWorldTexture instead of final screen
    //SDL_SetRenderTarget( renderTarget, gameWorldTexture );
    SDL_SetRenderTarget( renderTarget, preScaleTexture );

    SDL_Rect bg_rect = {0, 0, GamePanel::WIDTH, GamePanel::HEIGHT};
    SDL_RenderFillRect(renderTarget, &bg_rect);

    //Draw Background
    //bg->draw();

    //Draw Tilemap

    //Background and background objects
    //bgTileMap->draw();
	if (bg == NULL)
	{
		*(GamePanel::isRunningControl) = false;
	}
    bg->draw();

    //Player
    tileMap->draw();

    pig_player->draw();
    cake->draw();

/*
    //Setup to render to preScaleTexture instead of final screen
    SDL_SetRenderTarget( renderTarget, preScaleTexture );
    SDL_Rect gameWorldRect = {0, 36, GAME_WINDOW_W, 116};
    SDL_Rect gameWorldCrop = {GAME_WINDOW_W/4, 116/4+6, GAME_WINDOW_W/2, 116/2};
    SDL_RenderClear(renderTarget);
    if (!reachedSleepySam)
    {
        SDL_RenderCopy(renderTarget, gameWorldTexture, NULL, &gameWorldRect);
    }
    else
        SDL_RenderCopy(renderTarget, gameWorldTexture, &gameWorldCrop, &gameWorldRect);
*/

    //Set renderer back to main window
    SDL_SetRenderTarget( renderTarget, NULL );
    //SDL_Rect preScaleRect = {11, 12, GAME_WINDOW_W*3, GAME_WINDOW_H*3};

    int width_N = GamePanel::WINDOW_WIDTH/GAME_WINDOW_W;
    //int width_R = GamePanel::WINDOW_WIDTH%GAME_WINDOW_W;
    int height_N = GamePanel::WINDOW_HEIGHT/GAME_WINDOW_H;
    //int height_R = GamePanel::WINDOW_HEIGHT%152;
    int scale = width_N;
    if (width_N > height_N)
    {
        scale = height_N;
    }
    int width_R = GamePanel::WINDOW_WIDTH - GAME_WINDOW_W*scale;
    int height_R = GamePanel::WINDOW_HEIGHT - GAME_WINDOW_H*scale; 

    //SDL_Rect preScaleRect = {width_R/2, height_R/2, GAME_WINDOW_W*width_N, 152*height_N};
    SDL_Rect preScaleRect = {width_R/2, height_R/2, GAME_WINDOW_W*scale, GAME_WINDOW_H*scale};
    SDL_RenderCopy(renderTarget, preScaleTexture, NULL, &preScaleRect);
    //SDL_RenderCopy(renderTarget, gameWorldTexture, NULL, &scaleRect);

}
void PigState::keyPressed(int k)
{

    switch (k)
    {
        case SDLK_LEFT:
        {
            pig_player->setLeft(true);
            break;
        }
        case SDLK_RIGHT:
        {
            pig_player->setRight(true);
            break;
        }
        case SDLK_UP:
        {
            break;
        }
        case SDLK_DOWN:
        {
            break;
        }
        case SDLK_SPACE:
        {
            pig_player->setJumping(true);
            break;
        }
        case SDLK_LSHIFT:
        {
            //bgm->stop();
            //level_sounds[LEVEL_LOSE_SFX]->stop(-1);
            //gsm->setState(GameStateManager::TITLE_STATE);
            break;
        }
        case SDLK_RSHIFT:
        {
            bgm->stop();
            //level_sounds[LEVEL_LOSE_SFX]->stop(-1);
            *(GamePanel::isRunningControl) = false;
            break;
        }
        case SDLK_RETURN:
        {
            break;
        }
        case SDLK_z:
        {
            pig_player->dashAction();
            break;
        }
        case SDLK_c:
        {
            create_pressed = true;
            break;
        }
        case SDLK_l:
        {
            pig_player->loadStats("./Resources/Files/Pig_Stats.txt");
            break;
        }
    }
}
void PigState::keyReleased(int k)
{
    switch(k)
    {
        case SDLK_LEFT:
        {
            pig_player->setLeft(false);
            break;
        }
        case SDLK_RIGHT:
        {
            pig_player->setRight(false);
            break;
        }
        case SDLK_UP:
        {
            break;
        }
        case SDLK_DOWN:
        {
            break;
        }
        case SDLK_SPACE:
        {
            break;
        }
        case SDLK_LSHIFT:
        {
            break;
        }
        case SDLK_RSHIFT:
        {
            break;
        }
        case SDLK_RETURN:
        {
            break;
        }

    }
}


//----------------------------------------------------------


LevelCreator::LevelCreator(GameStateManager *gsm_, SDL_Renderer *renderTarget_)
{
    gsm = gsm_;
    renderTarget = renderTarget_;


    tileMap = NULL;
    bg = NULL;
    preScaleTexture = NULL;
}
LevelCreator::~LevelCreator()
{
    if (tileMap)
        delete tileMap;
    tileMap = NULL;

    if (bg)
        delete bg;
    bg = NULL;

    if (preScaleTexture)
        SDL_DestroyTexture(preScaleTexture);
    preScaleTexture = NULL;
}
void LevelCreator::init()
{
    tileMap = new TileMap(30, renderTarget);
    tileMap->loadTiles("./Resources/Tilesets/grasstileset.bmp", "");
    tileMap->makeMap(200, 200, "./Resources/Maps/creator_map.map");
    tileMap->setPosition(0, 0);
    tileMap->setTween(1);

    bg = new Background("./Resources/Backgrounds/grassbg1.bmp", 0.1, renderTarget);

    preScaleTexture = SDL_CreateTexture(renderTarget, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GAME_WINDOW_W, GAME_WINDOW_H);

    cursor_x = 0;
    cursor_y = 0;
    tile_idx = 0;
    map_name = "./Resources/Maps/creator_map.map";
    space_pressed = false;
    save_pressed = false;
    play_pressed = false; 
}
void LevelCreator::update()
{
    static int cnt = 0;

    if (cursor_x < 0)
    {
        cursor_x = 0;
    }
    if (cursor_x >= 200)
    {
        cursor_x = 200 - 1;
    }
    if (cursor_y < 0)
    {
        cursor_y = 0;
    }
    if (cursor_y >= 200)
    {
        cursor_y = 200 - 1;
    }

    if (tile_idx < 0)
    {
        tile_idx = 26 - 1;
    }
    if (tile_idx >= 26)
    {
        tile_idx = 0;
    }


    if (space_pressed)
    {
        tileMap->setMapIdx(cursor_y, cursor_x, tile_idx, true);
    }
    space_pressed = false;

    if (save_pressed)
    {
        tileMap->saveMap(map_name);
    }
    save_pressed = false;

    if (play_pressed)
    {
        gsm->setState(GameStateManager::PIG_STATE);
        return;
    }

    tileMap->setPosition(GamePanel::WIDTH/2 - cursor_x*30,
                         GamePanel::HEIGHT/2 - cursor_y*30);

    cnt++;
    if (cnt % 30 == 0)
    {
        int t_idx = tileMap->getMapIdx(5, 5);
        t_idx++; 
        t_idx %= 30;
        tileMap->setMapIdx(5, 5, t_idx, false);
    }

}
void LevelCreator::draw()
{
    static int cnt = 0;

    SDL_DisplayMode current;
    SDL_GetDesktopDisplayMode(0, &current);
    int window_width, window_height;

    SDL_SetRenderTarget(renderTarget, preScaleTexture);

	//Draw black background in case bg texture doesn't draw
	//SDL_SetRenderDrawColor(renderTarget, 0, 0, 0, 255);
    SDL_Rect bg_rect = {0, 0, GamePanel::WIDTH, GamePanel::HEIGHT};
    SDL_RenderFillRect(renderTarget, &bg_rect);

    bg->draw();
	bg->draw();

	//Flashing tile effect
    int t_idx = tileMap->getMapIdx(cursor_y, cursor_x);
    tileMap->setMapIdx(cursor_y, cursor_x, 0, false);
    if (cnt > 30)
    {
        tileMap->setMapIdx(cursor_y, cursor_x, tile_idx, false);
    }
    tileMap->draw();
    tileMap->setMapIdx(cursor_y, cursor_x, t_idx, false);

	uint8_t r_, g_, b_, a_;
	SDL_GetRenderDrawColor(renderTarget, &r_, &g_, &b_, &a_);

    //draw cursor
	if (tileMap->getTypeFromTileSet(tile_idx) == Tile::BLOCKED)
	{
		//If blocked set render color to blue
		SDL_SetRenderDrawColor(renderTarget, 255, 255, 255, 255);
	}
	else
	{
		//Set render color to black
		SDL_SetRenderDrawColor(renderTarget, 0, 0, 0, 255);
	}
    SDL_Rect rect = {(int)tileMap->getX() + cursor_x*30, (int)tileMap->getY() + cursor_y*30, 30, 30};
    SDL_RenderDrawRect(renderTarget, &rect);

	//Revent color
	SDL_SetRenderDrawColor(renderTarget, r_, g_, b_, a_);


    //Set renderer back to main window
    SDL_SetRenderTarget( renderTarget, NULL );
    
    int width_N = GamePanel::WINDOW_WIDTH/GAME_WINDOW_W;
    int height_N = GamePanel::WINDOW_HEIGHT/GAME_WINDOW_H;
    int scale = width_N;
    if (width_N > height_N)
    {
        scale = height_N;
    }   
    int width_R = GamePanel::WINDOW_WIDTH - GAME_WINDOW_W*scale;
    int height_R = GamePanel::WINDOW_HEIGHT - GAME_WINDOW_H*scale;

    SDL_Rect preScaleRect = {width_R/2, height_R/2, GAME_WINDOW_W*scale, GAME_WINDOW_H*scale};
    SDL_RenderCopy(renderTarget, preScaleTexture, NULL, &preScaleRect);

    cnt++;
    if (cnt >= 60)
    {
        cnt = 0;
    }
}
void LevelCreator::keyPressed(int k)
{
    switch (k)
    {
        case SDLK_LEFT:
        {
            cursor_x--;
            break;
        }
        case SDLK_RIGHT:
        {
            cursor_x++;
            break;
        }
        case SDLK_UP:
        {
            cursor_y--;
            break;
        }
        case SDLK_DOWN:
        {
            cursor_y++;
            break;
        }
        case SDLK_SPACE:
        {
            space_pressed = true;  
            break;
        }
        case SDLK_LSHIFT:
        {
            break;
        }
        case SDLK_RSHIFT:
        {
            *(GamePanel::isRunningControl) = false;
            break;
        }
        case SDLK_RETURN:
        {   
            break;
        }
        case SDLK_z:
        {   
            tile_idx--;
            break;
        }
        case SDLK_x:
        {
            tile_idx++;
            break;
        }
        case SDLK_s:
        {
            save_pressed = true;
            break;
        }
        case SDLK_p:
        {
            play_pressed = true;
            save_pressed = true;
            break;
        }
    }
}
void LevelCreator::keyReleased(int k)
{

}




