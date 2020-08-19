#include <stdio.h>

#include "../headers/TileMap.h"
#include "../headers/GamePanel.h"
#include "../headers/SDL_Helpers.h"

TileMap::TileMap(int tileSize_, SDL_Renderer *renderTarget_)
{
	renderTarget = renderTarget_;
	tileSize = tileSize_;
	numRowsToDraw = (GamePanel::HEIGHT / tileSize) + 2;
	numColsToDraw = (GamePanel::WIDTH / tileSize) + 2;
	tween = 1;//0.07;

    

	map = NULL;
	tiles = NULL;
	tileset = NULL;

	numTilesAcross = 0;
	numTilesVert = 0;
    x_max = 0;
    y_max = 0;
	fix_bounds = true;

    x = 0;
    y = 0;
}
TileMap::~TileMap()
{
	if (tileset)
		SDL_DestroyTexture(tileset);
	tileset = NULL;

	for (int i=0; i</*2*/numTilesVert; i++)
	{
		for (int j=0; j<numTilesAcross; j++)
		{
			if (tiles[i][j])
				delete tiles[i][j];
			tiles[i][j] = NULL;
		}
		if (tiles[i])
			delete[] tiles[i];
		tiles[i] = NULL;
	}
	if (tiles)
		delete[] tiles;
	tiles = NULL;

	for (int row=0; row<numRows; row++)
    {
		if (map[row])
			delete[] map[row];
		map[row] = NULL;
    }
	if (map)
		delete[] map;
	
}
void TileMap::loadTiles(std::string tilesheet_name, std::string tiletype_file)
{
	tileset = NULL;
	tileset = LoadTexture(tilesheet_name.c_str(), renderTarget);
	int tileset_width, tileset_height;
	SDL_QueryTexture(tileset, NULL, NULL, &tileset_width, &tileset_height);
	numTilesAcross = tileset_width / tileSize;
	numTilesVert = tileset_height / tileSize;
	//tiles = new Tile**[2];
	tiles = new Tile**[numTilesVert];
	for (int i=0; i</*2*/numTilesVert; i++)
	{
		tiles[i] = NULL;
		tiles[i] = new Tile*[numTilesAcross];
		for (int j=0; j<numTilesAcross; j++)
		{
			tiles[i][j] = NULL;
		}
	}

/*
	for (int col = 0; col < numTilesAcross; col++)
	{
		tiles[0][col] = new Tile(tileset, col*tileSize, 0,        tileSize, tileSize, Tile::NORMAL);
		tiles[1][col] = new Tile(tileset, col*tileSize, tileSize, tileSize, tileSize, Tile::BLOCKED);
	}
*/
    printf("Horizontal tile: %d, Vertical tiles: %d\n", numTilesAcross, numTilesVert);
	for (int col = 0; col < numTilesAcross; col++)
	{
		for (int row = 0; row <numTilesVert; row++)
		{
			tiles[row][col] = new Tile(tileset, col*tileSize, row*tileSize, tileSize, tileSize, Tile::NORMAL);
            if (row == 1)
            {
                tiles[row][col]->setType(Tile::BLOCKED);
            }
		}
	}	


/*
	int blocked_tiles[8] = {2, 4, 13, 22, 65, 66, 67, 68};
	for (int i=0; i<8; i++)
	{
		printf("tile = %d\n", blocked_tiles[i]);
		int rc = blocked_tiles[i];
    	int r = rc / numTilesAcross;
    	int c = rc % numTilesAcross;
		if (r >= numTilesVert || c >= numTilesAcross)
            continue;
    	tiles[r][c]->setType(Tile::BLOCKED);	
	}
*/
    FILE *fp = NULL;
	fp = fopen(tiletype_file.c_str(), "r");
	if (fp != NULL)
	{
		int tileCount;
		fscanf(fp, "%d ", &tileCount);
		for (int i=0; i<tileCount; i++)
		{
			int tileID;
			int tileType;
			fscanf(fp, "%d %d ", &tileID, &tileType);
			int rc = tileID;
        	int r = rc / numTilesAcross;
        	int c = rc % numTilesAcross;
        	if (r >= numTilesVert || c >= numTilesAcross)
			{
				printf("Error: Invalid tile\n");
         		continue;
			}

			//if (tileType == Tile::BLOCKED)
			//	tiles[r][c]->setType(Tile::BLOCKED);
			int heightMap[12];
			for (int i=0; i<12; i++)
			{
				fscanf(fp, "%d ", &(heightMap[i]));
				//printf("%d ", heightMap[i]);
			}
			printf("\n");
			tiles[r][c]->setType(tileType);
			if (tileType == Tile::HEIGHT_MAP)
			{
				tiles[r][c]->setHeightMap(heightMap);	
			}
		}
	}
    else
    {
        printf("Could not open tilemap_types file '%s'\n", tiletype_file.c_str());
    }

}	
void TileMap::loadMap(std::string s)
{
	FILE *fp = NULL;
	fp = fopen(s.c_str(), "r");
	if (fp == NULL)
	{
		printf("Failed to open file: %s\n", s.c_str());
		return;
	}
	
	fscanf(fp, "%d", &numCols);
	fscanf(fp, "%d", &numRows);
	width = numCols * tileSize;
	height = numRows * tileSize;

    printf("Horizontal tiles: %d, Vertical Tiles: %d\n", numCols, numRows);
    x_max = numCols;
    y_max = numRows;
    

	xmin = GamePanel::WIDTH - width;
	xmax = 0;
	ymin = GamePanel::HEIGHT - height;
	ymax = 0; 

	map = new int*[numRows];
	for (int row=0; row<numRows; row++)
	{
		map[row] = new int[numCols];
		for (int col=0; col<numCols; col++)
		{
			int temp;
			fscanf(fp, "%d", &temp);
			map[row][col] = temp;
			//printf("DEBUG: %d\n", temp);
		}
	}


	fclose(fp);
}
void TileMap::deleteMap()
{
    for (int row=0; row<numRows; row++)
    {
        if (map[row])
            delete[] map[row];
        map[row] = NULL;
    }
    if (map)
        delete[] map;
}
void TileMap::makeMap(int rows, int cols, std::string s)
{
    if (strlen(s.c_str()) == 0)
    {
        printf("Make empty map\n");
        //Empty map
        numRows = rows;
        numCols = cols;

        width = numCols * tileSize;
        height = numRows * tileSize;
    
        xmin = GamePanel::WIDTH - width;
        xmax = 0;
        ymin = GamePanel::HEIGHT - height;
        ymax = 0;

        if (map)
            deleteMap();
        map = new int*[numRows];
        for (int row=0; row < numRows; row++)
        {
            map[row] = new int[numCols];
            for (int col=0; col < numCols; col++)
                map[row][col] = 0;
        }
    }
    else
    {
        printf("Load map\n");
        //Load map
        loadMap(s);
    }
}
void TileMap::saveMap(std::string s)
{
    printf("Saving map...");
    FILE *fp = NULL;
    fp = fopen(s.c_str(), "w");
    if (fp == NULL)
    {
        printf("Failed to open file: %s\n", s.c_str());
        return;
    }

    fprintf(fp, "%d\n%d\n", x_max+1, y_max+1);
    
    for (int row=0; row<=y_max; row++)
    {
        for (int col=0; col<=x_max; col++)
        {
            fprintf(fp, "%d ", map[row][col]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
int TileMap::getMapIdx(int r, int c)
{
    return map ? map[r][c] : -1;
}
int TileMap::setMapIdx(int r, int c, int idx, bool track) 
{
    if (map == NULL)
        return -1;
    if (map[r] == NULL)
        return -1;
    map[r][c] = idx;
    if (track)
    {
        if (r > y_max)
        {
            y_max = r;
        }
        if (c > x_max)
        {
            x_max = c;
        }
    }
    return 0;
}

void TileMap::fixBounds()
{
	if (x < xmin)
		x = xmin;
	if (y < ymin)
		y = ymin;
	if (x > xmax)
		x = xmax;
	if (y > ymax)
		y = ymax;
}
int TileMap::getType(int row, int col)
{
	int rc = map[row][col];
	int r = rc / numTilesAcross;
	int c = rc % numTilesAcross;
	return tiles[r][c]->getType();
}
Tile* TileMap::getTile(int row, int col)
{
	if (row < 0 || col < 0 || row >= numRows || col >= numCols)
		return NULL;

	int rc = map[row][col];
	int r = rc / numTilesAcross;
	int c = rc % numTilesAcross;
	if (r >= numTilesVert || c >= numTilesAcross)
            return NULL;
	//printf("Tile ID: %d\n", rc);
	return tiles[r][c];
}
Tile* TileMap::getTileFromPos(int x, int y)
{
	int col = x/tileSize;
	int row = y/tileSize;
	//printf("Tile Row: %d, Tile Col: %d\n", row, col);
	return getTile(row, col);
}
bool TileMap::checkTileCollision(int x, int y)
{
	Tile *tile = getTileFromPos(x, y);
	if (!tile)
		return false;
	//printf("Tile Type: %d\n", tile->getType());
	return tile->checkCollision(x, y);
}
void TileMap::setPosition(double x_, double y_)
{
	x += (x_ - x) * tween;
	y += (y_ - y) * tween;

	if (fix_bounds)
		fixBounds();

	colOffset = (int)(-1*x) / tileSize;
	rowOffset = (int)(-1*y) / tileSize;
}
void TileMap::draw()
{
    //printf("Drawing tilemap\n");
	if (renderTarget == NULL)
		printf("DEBUG: Render target is NULL\n");
	for (int row = rowOffset; row  < rowOffset + numRowsToDraw; row++)
	{
		if (row >= numRows)
			break;
		for (int col = colOffset; col < colOffset + numColsToDraw; col++)
		{
			//printf("rowoffset=%d, coloffset=%d, numRowsToDraw=%d, numColsToDraw=%d\n", rowOffset, colOffset, numRowsToDraw, numColsToDraw);
            //printf("WIDTH = %d, HEIGHT = %d\n", GamePanel::WIDTH, GamePanel::HEIGHT);
			//printf("map[%d][%d] = %d\n\n", row, col, map[row][col]);
			if (col >= numCols)
				break;
			if (map[row][col] == 0)
				continue;
			int rc = map[row][col];
			int r = rc / numTilesAcross;
			int c = rc % numTilesAcross;
			SDL_Rect cropRect = tiles[r][c]->getCropRect();
			SDL_Rect posRect;
			posRect.x = (int)x + col*tileSize;
			posRect.y = (int)y + row*tileSize;
			posRect.w = tileSize;
			posRect.h = tileSize;

			SDL_RenderCopy(renderTarget, tiles[r][c]->getImage(), &cropRect, &posRect);
            
            SDL_Rect rect = posRect; 
            Uint8 r_, g, b, a;
            SDL_GetRenderDrawColor(renderTarget, &r_, &g, &b, &a);
            SDL_SetRenderDrawColor(renderTarget, 255, 0, 0, 100);
            if (tiles[r][c]->getType() == Tile::BLOCKED)
            {
               // SDL_RenderFillRect(renderTarget, &rect);
                //printf("Tile[%d][%d] blocked\n", row, col);
            }
            SDL_SetRenderDrawColor(renderTarget, r_, g, b, a);
		}
	}
}


