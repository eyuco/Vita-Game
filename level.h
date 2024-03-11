#include "vec.h"

void tileObjs();
vector getTilesets();
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

struct Level{

    //void update();
   // void render();
};
//struct Tileset* m_tilesets = vector_create();
void tileObjs(){
    struct Tileset* m_tilesets = vector_create();
}
vector getTilesets()
{

}
