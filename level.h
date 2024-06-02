#include "vec.h"

void tileObjs();

//might need to make grid and draw the columns that way
vector getTilesets();
void levelRender();
struct Tileset{
  int firstGridID;
int tileWidth;
int tileHeight;
int spacing;
int margin;
int width;
int height;
int numColumns;
char name[];
};
struct Layer{


};
struct Level{

    //void update();
   // void render();
};
//struct Tileset* m_tilesets = vector_create();
void tileObjs(){
    struct Tileset* m_tilesets = vector_create();
    struct Layer* m_layers = vector_create();

}
vector getTilesets()
{

}
void levelRender()
{
   //level consists of background and tile sprites

}
