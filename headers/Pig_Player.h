#pragma once
#include <SDL2/SDL.h>
#include <list>
#include "../headers/MapObject.h"
#include "../headers/AudioPlayer.h"


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
		static constexpr double dashSpeed=12.0;//15.0;
		static const int DASH_LEN = 125;//200;

		static constexpr float DRIFT_VAL=2.5;//5;
	
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

		void reset(double x_, double y_);
        void update();
        void draw();
};
