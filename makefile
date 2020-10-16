OBJS = 	./src/HamJam_Platformer.o \
        ./src/SDL_Helpers.o \
        ./src/GamePanel.o \
        ./src/GameStateManager.o \
        ./src/GameState.o \
        ./src/Background.o \
        ./src/TileMap.o \
        ./src/Tile.o \
        ./src/MapObject.o \
        ./src/Enemy.o \
        ./src/Animation.o \
        ./src/HUD.o \
        ./src/Explosion.o \
        ./src/AudioPlayer.o \
        ./src/BgObject.o \
        ./src/TriggerBox.o \
        ./src/Numbers.o \
        ./src/InputBuffer.o \
        ./src/Letters.o \
        ./src/Pig_Player.o \
        ./src/Cake.o


ifeq ($(OS), Windows_NT)
$(info "Compiling on Windows...")
SHELL=cmd
IPATH = -IC:\mingw_dev_lib\include -LC:\mingw_dev_lib\lib -w -Wl,-subsystem,windows
LFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer
PRE = ./src/
BUILDDIR = ./build-win/
RMCMD = del	.\src\*.o
else
IPATH = 
LFLAGS = -lSDL2 -lSDL2_mixer
PRE = 
BUILDDIR = 
RMCMD = rm $(OBJS)
endif

CC = g++
DEBUG = -g
CFLAGS = -Wall $(DEBUG)
DS = ./src/
DH = ./headers/


HamJam_Platformer: $(OBJS)
	$(CC) $(CFLAGS) $(IPATH) $(OBJS) $(LFLAGS) -o $(BUILDDIR)HamJam_Platformer


$(PRE)SDL_Helpers.o: $(DS)SDL_Helpers.cpp $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)SDL_Helpers.cpp $(IPATH) -o $(DS)SDL_Helpers.o

$(PRE)GamePanel.o: $(DS)GamePanel.cpp $(DH)GamePanel.h $(DH)AudioPlayer.h
	$(CC) -c $(CFLAGS) $(DS)GamePanel.cpp $(IPATH) -o $(DS)GamePanel.o

$(PRE)GameStateManager.o: $(DS)GameStateManager.cpp $(DH)GameStateManager.h $(DH)GameState.h
	$(CC) -c $(CFLAGS) $(DS)GameStateManager.cpp $(IPATH) -o $(DS)GameStateManager.o

$(PRE)Background.o: $(DS)Background.cpp $(DH)Background.h $(DH)GamePanel.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Background.cpp $(IPATH) -o $(DS)Background.o

$(PRE)GameState.o: $(DS)GameState.cpp $(DH)GameStateManager.h $(DH)GameState.h $(DH)Background.h\
			 $(DH)TileMap.h $(DH)SDL_Helpers.h $(DH)Player.h $(DH)Enemy.h $(DH)HUD.h\
             $(DH)Explosion.h $(DH)AudioPlayer.h $(DH)BgObject.h $(DH)TriggerBox.h $(DH)Letters.h
	$(CC) -c $(CFLAGS) $(DS)GameState.cpp $(IPATH) -o $(DS)GameState.o

$(PRE)Tile.o: $(DS)Tile.cpp $(DH)Tile.h
	$(CC) -c $(CFLAGS) $(DS)Tile.cpp $(IPATH) -o $(DS)Tile.o

$(PRE)TileMap.o: $(DS)TileMap.cpp $(DH)TileMap.h $(DH)Tile.h $(DH)GamePanel.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)TileMap.cpp $(IPATH) -o $(DS)TileMap.o

$(PRE)MapObject.o: $(DS)MapObject.cpp $(DH)MapObject.h $(DH)TileMap.h $(DH)Tile.h $(DH)GamePanel.h $(DH)SDL_Helpers.h\
			$(DH)Animation.h
	$(CC) -c $(CFLAGS) $(DS)MapObject.cpp $(IPATH) -o $(DS)MapObject.o

$(PRE)Animation.o: $(DS)Animation.cpp $(DH)Animation.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Animation.cpp $(IPATH) -o $(DS)Animation.o

$(PRE)Pig_Player.o: $(DS)Pig_Player.cpp $(DH)Pig_Player.h $(DH)MapObject.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Pig_Player.cpp $(IPATH) -o $(DS)Pig_Player.o  

$(PRE)Cake.o: $(DS)Cake.cpp $(DH)Cake.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)AudioPlayer.h
	$(CC) -c $(CFLAGS) $(DS)Cake.cpp $(IPATH) -o $(DS)Cake.o

$(PRE)Enemy.o: $(DS)Enemy.cpp $(DH)Enemy.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)TriggerBox.h
	$(CC) -c $(CFLAGS) $(DS)Enemy.cpp $(IPATH) -o $(DS)Enemy.o

$(PRE)HUD.o: $(DS)HUD.cpp $(DH)HUD.h $(DH)Player.h $(DH)MapObject.h $(DH)SDL_Helpers.h $(DH)FireBall.h
	$(CC) -c $(CFLAGS) $(DS)HUD.cpp $(IPATH) -o $(DS)HUD.o

$(PRE)Explosion.o: $(DS)Explosion.cpp $(DH)Explosion.h $(DH)SDL_Helpers.h $(DH)Animation.h
	$(CC) -c $(CFLAGS) $(DS)Explosion.cpp $(IPATH) -o $(DS)Explosion.o

$(PRE)AudioPlayer.o: $(DS)AudioPlayer.cpp $(DH)AudioPlayer.h
	$(CC) -c $(CFLAGS) $(DS)AudioPlayer.cpp $(IPATH) -o $(DS)AudioPlayer.o

$(PRE)BgObject.o: $(DS)BgObject.cpp $(DH)BgObject.h $(DH)MapObject.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)BgObject.cpp $(IPATH) -o $(DS)BgObject.o

$(PRE)TriggerBox.o: $(DS)TriggerBox.cpp $(DH)TriggerBox.h $(DH)MapObject.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)TriggerBox.cpp $(IPATH) -o $(DS)TriggerBox.o

$(PRE)Numbers.o: $(DS)Numbers.cpp $(DH)Numbers.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Numbers.cpp $(IPATH) -o $(DS)Numbers.o

$(PRE)InputBuffer.o: $(DS)InputBuffer.cpp $(DH)InputBuffer.h
	$(CC) -c $(CFLAGS) $(DS)InputBuffer.cpp $(IPATH) -o $(DS)InputBuffer.o

$(PRE)Letters.o: $(DS)Letters.cpp $(DH)Letters.h $(DH)SDL_Helpers.h
	$(CC) -c $(CFLAGS) $(DS)Letters.cpp $(IPATH) -o $(DS)Letters.o

$(PRE)HamJam_Platformer.o: $(DS)HamJam_Platformer.cpp $(DH)SDL_Helpers.h $(DH)GamePanel.h
	$(CC) -c $(CFLAGS) $(DS)HamJam_Platformer.cpp $(IPATH) -o $(DS)HamJam_Platformer.o


clean:
	$(RMCMD)

