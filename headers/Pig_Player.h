#pragma once
#include <SDL2/SDL.h>
#include <list>
#include <string>
#include "../headers/MapObject.h"
#include "../headers/AudioPlayer.h"

typedef enum PIG_STATS {DASH_SPEED, DASH_LEN_MAP, DRIFT_VAL_MAP, MAX_SPEED, 
                        MOVE_SPEED, STOP_SPEED, FALL_SPEED, MAX_FALL_SPEED,
                        JUMP_START_MAP, NUM_STATS} pig_stats_t;

typedef struct stat_map_el
{
    pig_stats_t stat;
    char stat_str[64];
} stat_map_element_t;

class Pig_Player : public MapObject
{
	private:
		bool dead;
		bool onGround;
		bool visible;
		bool jumpInit;	
		bool canDoubleJump;
		bool landing;

		int cOffset_x;
		int cOffset_y;


        enum {BOTTOM_LEFT, BOTTOM_RIGHT, NUM_OFFSETS};

        static constexpr int collider_offsets[NUM_OFFSETS][2]  = {
                                                    {-8, 24},
                                                    {10, 24},
                                                  };


		bool canDash;
		bool dashStart;
		bool dash_end;
		bool dashing;
		double dashStartX;
		double dashStartY;
		double dashSpeed;//=12.0;//15.0;
		int dash_len;// = 125;//200;

		float drift_val;//=2.5;//5;


        //Default values
        static constexpr float  DEF_DASH_SPEED = 12.0f;
        static const int        DEF_DASH_LEN = 125;
        static constexpr float  DEF_DRIFT_VAL = 2.5f;
        static const int        DEF_MAX_SPEED = 6;
        static constexpr float  DEF_MOVE_SPEED = 0.35f;
        static constexpr float  DEF_STOP_SPEED = 0.5f;
        static constexpr float  DEF_FALL_SPEED = 0.35f;
        static constexpr float  DEF_MAX_FALL_SPEED = 15.0f;
        static const int        DEF_JUMP_START = -8;

        stat_map_element_t stat_map[NUM_STATS] = {
            {DASH_SPEED, "dashSpeed"},
            {DASH_LEN_MAP, "dashLen"},
            {DRIFT_VAL_MAP, "drift_val"},
            {MAX_SPEED, "maxSpeed"},
            {MOVE_SPEED, "moveSpeed"},
            {STOP_SPEED, "stopSpeed"},
            {FALL_SPEED, "fallSpeed"},
            {MAX_FALL_SPEED, "maxFallSpeed"},
            {JUMP_START_MAP, "jumpStart"},
        };

	
		//Animations
		int numAnimations;
		SDL_Texture *animationTexture;
		SDL_Rect **sprite_rects;
		int *frameNumbers;

		//Audio
		AudioPlayer **player_sounds;
	
		void getNextPosition();
		void initAnimation();

	public:
		enum Player_Actions {STANDING, WALKING, JUMP_START, JUMP_PEAK, FALLING, LANDING, BRAKING_F, BRAKING_R, DASH_START, DASHING, DASH_STOP, SLIDE, ANM_NUM};
		enum Player_Sounds {JUMP_SFX, SFX_NUM};

		Pig_Player(TileMap *tm, SDL_Renderer *renderTarget_);
        ~Pig_Player();

		bool isDead() {return dead;}
		void setDead(bool b) {dead = b;}
		bool isOnGround() {return onGround;}
		bool getVisible() {return visible;}
		void setVisible(bool b) {visible = b;}
		void dashAction();
		void dashStop();
		void setJumping(bool b);		
		void recharge(){canDash = true; canDoubleJump = true;}
		SDL_Rect getCollisionRect();
        int loadStats(std::string s);

		void reset(double x_, double y_);
        void update();
        void draw();
};
