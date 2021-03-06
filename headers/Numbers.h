#pragma once
#include <SDL2/SDL.h>

class Numbers
{
	private:
		int digits;
		int spacing;
		int x, y;
		bool visible;
		int xOffset, yOffset;
		static const int MAX_DIGITS=10;
		static const int NUM_WIDTH=6;
		static const int NUM_HEIGHT = 13;		

		SDL_Texture *numberTexture;
		SDL_Renderer *renderTarget;

	public:
		Numbers(SDL_Renderer *renderTarget_, int digits_, int spacing_, int x_, int y_, bool visible_=true);
		~Numbers();
		int getDigits() {return digits;}
		void setDigits(int d) {digits = d;}
		int getSpacing() {return spacing;}
		void setSpacing(int s) {spacing = s;}
		bool getVisible() {return visible;}
		void setVisible(bool b) {visible = b;}
		int getX() {return x;}
		int setX(int x_) {x = x_; return 0;}
		int getY() {return y;}
		int setY(int y_) {y = y_; return 0;}
		int getXOffset() {return xOffset;}
        void setXOffset(int offset) {xOffset = offset;}
        int getYOffset() {return yOffset;}
        void setYOffset(int offset) {yOffset = offset;}
		int getWidth() {return digits*NUM_WIDTH + (digits-1)*spacing;}
		int getHeight() {return NUM_HEIGHT;}
		void draw(int number);
};
