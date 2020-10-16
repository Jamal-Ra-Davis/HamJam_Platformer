#include <time.h>
#include <string.h>

#include "../headers/Pig_Player.h"
#include "../headers/SDL_Helpers.h"
#include "../headers/GamePanel.h"


Pig_Player::Pig_Player(TileMap *tm, SDL_Renderer *renderTarget_) : MapObject(tm, renderTarget_)
{
	dead = false;
	onGround = false;
	falling = true;	
	canDoubleJump = true;
	canDash = true;
	dashing = false;
	dashStart = false;
	dash_end = false;	
	landing = false;
	
	visible = true;	


	width = 60;
    height = 50;
    cwidth = 40;
    cheight = 30;//27;
	cOffset_x = 0;
	cOffset_y = 9;	


    dashSpeed = DEF_DASH_SPEED;
    dash_len = DEF_DASH_LEN;
    drift_val = DEF_DRIFT_VAL;

    maxSpeed = DEF_MAX_SPEED;
	moveSpeed = DEF_MOVE_SPEED;//maxSpeed;
    stopSpeed = DEF_STOP_SPEED;//0.35;//maxSpeed;

    fallSpeed = DEF_FALL_SPEED;
    maxFallSpeed = DEF_MAX_FALL_SPEED;
    jumpStart = DEF_JUMP_START;//-10;

    facingRight = true;

	//jumpInit = false;

	//load sprites
    numAnimations = ANM_NUM;
    frameNumbers = NULL;
    frameNumbers = new int[numAnimations];
    int tempNums[ANM_NUM] = {1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 1};
    for (int i=0; i<numAnimations; i++)
        frameNumbers[i] = tempNums[i];

    sprite_rects = NULL;
    sprite_rects = new SDL_Rect*[numAnimations];
    for (int i=0; i<numAnimations; i++)
    {
        sprite_rects[i] = NULL;
        sprite_rects[i] = new SDL_Rect[frameNumbers[i]];
        //printf("DEBUG: frameNumbers[%d] = %d\n", i, frameNumbers[i]);
    }
	
	animationTexture = LoadTexture("./Resources/Sprites/Player/Pig_spritesheet.bmp", renderTarget);
    if (animationTexture)
        printf("DEBUG: Loaded player textures\n");
    for (int i=0; i<numAnimations; i++)
    {
        for (int j=0; j<frameNumbers[i]; j++)
        {
         	sprite_rects[i][j].x = j*width;
        	sprite_rects[i][j].y = i*height;
        	sprite_rects[i][j].w = width;
        	sprite_rects[i][j].h = height;
        }
    }

	initAnimation();

	//Audio loading
    player_sounds = NULL;
    player_sounds = new AudioPlayer*[SFX_NUM];
    for (int i=0; i<SFX_NUM; i++)
    {
        player_sounds[i] = NULL;
    }
    player_sounds[JUMP_SFX] = new AudioPlayer("./Resources/SFX/HamJam_Jump.wav", false);
	
}
Pig_Player::~Pig_Player()
{
    delete frameNumbers;
    frameNumbers = NULL;
    for (int i=0; i<numAnimations; i++)
    {
        delete [] sprite_rects[i];
        sprite_rects[i] = NULL;
    }
    delete [] sprite_rects;
    sprite_rects = NULL;

    SDL_DestroyTexture(animationTexture);
    animationTexture = NULL;

/*
    for (int i=0; i<SFX_NUM; i++)
    {
        delete player_sounds[i];
        player_sounds[i] = NULL;
    }
    delete player_sounds;
    player_sounds = NULL;
*/
}
void Pig_Player::getNextPosition()
{
	//Fall
   	dy += fallSpeed;
/*
	if (onGround)
	{
    	if (left)
    	{
			if (dx > 0)
        		dx -= moveSpeed*2;
			else
				dx -= moveSpeed;
        	if (dx < -maxSpeed)
        	{
            	dx = -maxSpeed;
        	}	
    	}
    	else if (right)
    	{
			if (dx < 0)
				dx += moveSpeed*2;
			else
        		dx += moveSpeed;
        	if (dx > maxSpeed)
        	{
            	dx = maxSpeed;
        	}
   		}
    	else
    	{
        	if (dx > 0)
        	{
            	dx -= stopSpeed;
            	if (dx < 0)
                	dx = 0;
        	}
        	else if (dx < 0)
        	{
            	dx += stopSpeed;
            	if (dx > 0)
                	dx = 0;
        	}
    	}
	}
	else
	{
		if (left)
		{
			dx -= moveSpeed/drift_val;
			if (dx < -maxSpeed)
			{
				dx = -maxSpeed;
			}
		}
		else if (right)
		{
			dx += moveSpeed/drift_val;
            if (dx > maxSpeed)
            {
                dx = maxSpeed;
            }
		}
	}
*/
	if (dashing)
	{
		dy = 0;
		if (dashStart)
		{
			if (left)
			{
				dx = -dashSpeed;
			}
			else if (right)
			{
				dx = dashSpeed;
			}
			else if (facingRight)
			{
				dx = dashSpeed;
			}
			else
			{
				dx = -dashSpeed;
			}
		}
	}
	else if (jumping)
	{
		if (onGround)
		{
			onGround = false;
			dy = jumpStart;
            /*
            if (right)
            {
                dx += maxSpeed*0.5;
                if (dx > maxSpeed)
                    dx = maxSpeed;
            }
            else if (left)
            {
                dx -= maxSpeed*0.5;
                if (dx < -maxSpeed)
                    dx = -maxSpeed;
            }
            */
		}
		else if (canDoubleJump)
		{
			canDoubleJump = false;
			dy = jumpStart*0.85;
			if (right)
			{
				/*
				if (dx >= 0)
					dx = dx;
				else 
					dx = -dx;
				*/
				dx = maxSpeed*0.75;
			}
			else if (left)
			{	
				/*
				if (dx <= 0)
					dx = dx;
				else
					dx = -dx;
				*/
				dx = -maxSpeed*0.75;
			}
			else
				dx = 0;	
		}
	}
	else
	{
		if (/*!isBusy()*/onGround)
    	{
        	if (left)
        	{
            	if (dx > 0)
                	dx -= moveSpeed*1.5;
            	else
                	dx -= moveSpeed;
            	if (dx < -maxSpeed)
            	{
               	 	dx = -maxSpeed;
            	}	
        	}
        	else if (right)
        	{
            	if (dx < 0)
                	dx += moveSpeed*1.5;
            	else
                	dx += moveSpeed;
            	if (dx > maxSpeed)
            	{
                	dx = maxSpeed;
            	}
        	}
        	else
        	{
            	if (dx > 0)
            	{
                	dx -= stopSpeed;
                	if (dx < 0)
                    	dx = 0;
            	}
            	else if (dx < 0)
            	{
                	dx += stopSpeed;
                	if (dx > 0)
                    	dx = 0;
            	}
        	}
    	}
		else
		{
			 if (left)
        	{
            	dx -= moveSpeed/drift_val;
            	if (dx < -maxSpeed)
            	{
                	dx = -maxSpeed;
            	}
            }
        	else if (right)
        	{
            	dx += moveSpeed/drift_val;
            	if (dx > maxSpeed)
            	{
                	dx = maxSpeed;
            	}
        	}
		}
	}

	if (dy > maxFallSpeed)
        dy = maxFallSpeed;

	if (!dashing)
	{
		if (dx < -maxSpeed)
			dx = -maxSpeed;
		if (dx > maxSpeed)
			dx = maxSpeed;
	}

/*
	//falling
    if (!dashing)
    {
        dy += fallSpeed;
        if (dy > maxFallSpeed)
            dy = maxFallSpeed;
    }
*/
}
void Pig_Player::initAnimation()
{
	currentAction = STANDING;
    animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
    animation.setDelay(-1);
    width = sprite_rects[currentAction][animation.getFrame()].w;
}
void Pig_Player::dashAction()
{
	if (canDash && !dashing)
	{
		dashStart = true;
		dashStartX = x;
		dashStartY = y;
		dashing = true;
		jumping = false;
		canDash = false;
		//set dash start animation
	}
}
void Pig_Player::dashStop()
{
	dashing = false;
	//dx = 0;
}
void Pig_Player::setJumping(bool b)
{
	if (!canDoubleJump)
		return;
	jumping = b;
	if (jumping)
	{
		player_sounds[JUMP_SFX]->play(0);
		dashStop();
	}
}
SDL_Rect Pig_Player::getCollisionRect()
{
	//printf("In Pig collision rect funtion\n");
	SDL_Rect cRect;
    cRect.x = (int)(x - cwidth/2) + cOffset_x;
    cRect.y = (int)(y - cheight/2) + cOffset_y;
    cRect.w = cwidth;
    cRect.h = cheight;
    return cRect;
}
int Pig_Player::loadStats(std::string s)
{
    FILE *stat_file = NULL;
    char buf[128];

    stat_file = fopen(s.c_str(), "r");
    if (stat_file == NULL)
    {
        printf("Failed to open file: %s\n", s.c_str());
        return -1;
    }
    while(fgets(buf, 128, stat_file) != NULL)
    {
        printf("Line: '%s'\n", buf);
        if (buf[0] == '#')
        {
            continue;
        }
        
        char str_flag[64] = {'\0'};
        int ret = sscanf(buf, "%s ", str_flag);
        printf("Parsed data: str_flag = %s, return = %d\n", str_flag, ret);
        pig_stats_t stat = NUM_STATS;
        for (int i = 0; i<NUM_STATS; i++)
        {
            if (strcmp(str_flag, stat_map[i].stat_str) == 0)
            {   
                printf("Stat match!\n");
                stat = stat_map[i].stat;
                break;
            }
        } 
        printf("STAT: %d\n", stat);

        switch(stat)
        {
            case DASH_SPEED:
            {
                float dash_speed_;
                ret = sscanf(buf, "%s %f", str_flag, &dash_speed_);
                if (ret == 2)
                {
                    dashSpeed = dash_speed_;
                }
                break;
            }
            case DASH_LEN_MAP:
            {
                int dash_len_;
                ret = sscanf(buf, "%s %d", str_flag, &dash_len_);
                if (ret == 2)
                {
                    dash_len = dash_len_;
                }
                break;
            }
            case DRIFT_VAL_MAP:
            {
                float drift_val_;
                ret = sscanf(buf, "%s %f", str_flag, &drift_val_);
                if (ret == 2)
                {
                    drift_val = drift_val_;
                }
                break;
            }
            case MAX_SPEED:
            {
                int max_speed_;
                ret = sscanf(buf, "%s %d", str_flag, &max_speed_);
                if (ret == 2)
                {
                    maxSpeed = max_speed_;
                }
                break;
            }
            case MOVE_SPEED:
            {
                float move_speed_;
                ret = sscanf(buf, "%s %f", str_flag, &move_speed_);
                if (ret == 2)
                {
                    moveSpeed = move_speed_;
                }
                break;
            }
            case STOP_SPEED:
            {
                float stop_speed_;
                ret = sscanf(buf, "%s %f", str_flag, &stop_speed_);
                if (ret == 2)
                {
                    stopSpeed = stop_speed_;
                }
                break;
            }
            case FALL_SPEED:
            {
                float fall_speed_;
                ret = sscanf(buf, "%s %f", str_flag, &fall_speed_);
                if (ret == 2)
                {
                    fallSpeed = fall_speed_;
                }
                break;
            }
            case MAX_FALL_SPEED:
            {
                float max_fall_speed_;
                ret = sscanf(buf, "%s %f", str_flag, &max_fall_speed_);
                if (ret == 2)
                {
                    maxFallSpeed = max_fall_speed_;
                }
                break;
            }
            case JUMP_START_MAP:
            {
                int jump_start_;
                ret = sscanf(buf, "%s %d", str_flag, &jump_start_);
                if (ret == 2)
                {
                    jumpStart = jump_start_;
                }
                break;
            }
            default:
            {
                break;
            }
            
        }      
    }

    return 0;
}
void Pig_Player::reset(double x, double y)
{
    dead = false;
    onGround = false;
	visible = true;
	canDoubleJump = true;
	canDash = true;
    dashing = false;
	dashStart = false;
    dash_end = false;
	landing = false;	

	left = false;
    right = false;
    up = false;
    down = false;
    jumping = false;
    falling = false;
	
	setVector(0, 0);
	setPosition(x, y);

	initAnimation();
}
void Pig_Player::update()
{
	//update position
    getNextPosition();
	xdest = x + dx;
	ydest = y + dy;


	onGround = false;
/*
	if (xdest - cwidth/2 + cOffset_x < 0)
	{
		xdest = cwidth/2 - cOffset_x;
		dx = 0;
		if (dashing)
		{
			dashStop();
			dash_end = true;
		}
	}
*/
/*
	else if (xdest + cwidth/2 + cOffset_x > GamePanel::WINDOW_WIDTH)
	{
		xdest = GamePanel::WINDOW_WIDTH - cwidth/2 - cOffset_x;
		dx = 0;
		if (dashing)
		{
			dashStop();
			dash_end = true;
		}
	}
*/
/*
	if (ydest + cheight/2 + cOffset_y > GamePanel::WINDOW_HEIGHT - 27) 
	{
		if (dy > -jumpStart/4)
		{
			landing = true;
		}

		ydest = GamePanel::WINDOW_HEIGHT - cheight/2 - cOffset_y - 27;
		dy = 0;
		onGround = true;
		canDoubleJump = true;
		canDash = true;
		if (dashing)
			dashStop();
	}
*/


    float x_left = xdest - cwidth/2 + 0;
    float x_right = xdest + cwidth/2 + 0;
    if (tileMap->checkTileCollision(x_left, y))
    {
        int targ = (int)(x_left/30);
        if ((int)(x_left) % 30)
        {
            targ++;
        }
        targ *= 30;
        float diff = targ - x_left + 0.1;
        xdest += diff;
        dx = 0;
        if (dashing)
        {
            dashStop();
            dash_end = true;
        }
    }
    else if (tileMap->checkTileCollision(x_right, y))
    {
        int targ = (int)(x_right/30);
        targ *= 30;
        float diff = targ - x_right - 0.1;
        xdest += diff;
        dx = 0;
        if (dashing)
        {
            dashStop();
            dash_end = true;
        }
    }



    float y_bottom = ydest + cheight/2 + 9;
    float y_top = ydest - cheight/2 + 5;
    //printf("**ydest = %f, bottom = %f, dy = %f\n", ydest, y_, dy);
    if (tileMap->checkTileCollision(x, y_bottom))
    {
        int targ = (int)(y_bottom/30) * 30;
        float diff = targ - y_bottom - 0.1;
        //printf("Target = %d, y_ = %f, diff = %f\n", targ, y_, diff);
        //printf("Collision!!\n");
        ydest += diff;
        //ydest -= .1;
        dy = 0;
        onGround = true;
        canDoubleJump = true;
        canDash = true;
        if (dashing)
            dashStop();
        //y--;
        //printf("##ydest = %f, bottom = %f\n", ydest, ydest + cheight/2 + 9);
    }
    else if (tileMap->checkTileCollision(x, y_top))
    {
        int targ = (int)(y_top/30);
        if ((int)(y_top) % 30)
        {
            targ++;
        }
        targ *= 30;
        float diff = targ - y_top + 0.1;
        //printf("Target = %d, y_ = %f, diff = %f\n", targ, y_, diff);
        //printf("Collision!!\n");
        ydest += diff;
        //ydest -= .1;
        dy = 0;
        //if (dashing)
        //    dashStop();

    }

/*
	if (dashing)
	{
		//printf("Dashing\n");
		dashStart = false;
		double delta = x - dashStartX;
		if (delta < 0)
			delta *= -1;
		if (delta >= dash_len)
		{
			dashStop();
		}
	}
*/	
	//if (onGround)
	//	printf("On Ground : %lf\n", getMs());

	x = xdest;
	y = ydest;

	if (dashing)
	{
		if (dashStart)
		{
			dashStart = false;
			currentAction = DASH_START;
        	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
        	animation.setDelay(50);
        	width = sprite_rects[currentAction][animation.getFrame()].w;
		}
		else
		{
			if (currentAction != DASHING)
			{
				currentAction = DASHING;
           		animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            	animation.setDelay(50);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
			}
		}
		double delta = x - dashStartX;
        if (delta < 0)
            delta *= -1;
        if (delta >= dash_len)
        {
            dashStop();
			//dash_end = true;
        }
	}
	else if (dash_end)
	{
		if (currentAction != DASH_STOP)
		{
			currentAction = DASH_STOP;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(100);
            width = sprite_rects[currentAction][animation.getFrame()].w;
		}	
		if (animation.hasPlayedOnce())
		{
			dash_end = false;
		}
	}
	else if (jumping)
	{
		currentAction = JUMP_START;
       	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
       	animation.setDelay(100);
       	width = sprite_rects[currentAction][animation.getFrame()].w;
	}
	else if (!onGround && dy > 0)
	{
		if (currentAction != FALLING)
		{
			currentAction = FALLING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            animation.setDelay(75);
            width = sprite_rects[currentAction][animation.getFrame()].w;
		}	
	}
	else if (currentAction == JUMP_START)
	{
		if (animation.hasPlayedOnce())
		{
			currentAction = JUMP_PEAK;
        	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
        	animation.setDelay(75);
        	width = sprite_rects[currentAction][animation.getFrame()].w;
		}
	}
	else if (currentAction == JUMP_PEAK)
	{
		
	}
	else if (landing)
	{
		if (currentAction != LANDING)
		{
			currentAction = LANDING;
            animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction], true);
            animation.setDelay(35);
            width = sprite_rects[currentAction][animation.getFrame()].w;
		}
		else if (animation.hasPlayedOnce())
		{
			landing = false;
		}
	}
	else if (onGround && (left || right))
	{
		if ((facingRight && dx < 0) || (!facingRight && dx > 0))
		{
			if (currentAction != SLIDE)
            {
                currentAction = SLIDE;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
                animation.setDelay(-1);
                width = sprite_rects[currentAction][animation.getFrame()].w;
            }
		}
		else
		{ 
			if (currentAction != WALKING)
			{
				currentAction = WALKING;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            	animation.setDelay(100);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
			}
		}
	}
	else if (onGround && dx != 0)
	{
		if ((facingRight && dx > 0) || (!facingRight && dx < 0))
		{
			if (currentAction != BRAKING_F)
			{
				currentAction = BRAKING_F;
            	animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
            	animation.setDelay(-1);
            	width = sprite_rects[currentAction][animation.getFrame()].w;
			}
		}
		else if ((facingRight && dx < 0) || (!facingRight && dx > 0))
		{
			if (currentAction != BRAKING_R)
            {
                currentAction = BRAKING_R;
                animation.setFrames(animationTexture, sprite_rects[currentAction], frameNumbers[currentAction]);
                animation.setDelay(-1);
                width = sprite_rects[currentAction][animation.getFrame()].w;
            }
		}
		else
		{
			if (currentAction != STANDING)
        	{
            	initAnimation();
        	}
		}
	}
	else
	{
		if (currentAction != STANDING)
		{
			initAnimation();
		}		
	}


	jumping = false;
	animation.update();
	if (/*onGround*/!dashing)
	{
        if (right)
            facingRight = true;
        else if (left)
            facingRight = false;
	}
	else
	{
		if (dx >= 0)
			facingRight = true;
		else
			facingRight = false;	
	}
}
void Pig_Player::draw()
{
	if (!visible)
		return;

    setMapPosition();
	SDL_Rect rect = this->getCollisionRect();
	drawRect(rect);
    mapObjectDraw();
/*
	SDL_Rect posRect = {(int)(x - width/2), (int)(y - height/2), width, height};
	SDL_Rect cropRect = animation.getImageRect();
	if (facingRight)
    {
        SDL_RenderCopyEx(renderTarget, animation.getFrameTexture(), &cropRect, &posRect, 0, NULL, SDL_FLIP_NONE);
    }
    else
    {
        SDL_RenderCopyEx(renderTarget, animation.getFrameTexture(), &cropRect, &posRect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
*/
}

