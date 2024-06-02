#include <stdbool.h>
#include <vita2d.h>

extern unsigned char _binary_menuBG_png_start;
extern unsigned char _binary_cursor_png_start;
#define NUM_OPTIONS 3
#define WALKING_ANIMATION_FRAMES  2
bool menuState;
int currentIndex = 0;
vita2d_texture *cursor;
vita2d_texture *menu_BG;

struct Entity {
    float x;
    float y;
    float w;
    float h;
    float dx;
    float dy;
    int health;
    int reload;
    vita2d_texture *image[WALKING_ANIMATION_FRAMES];
    //Entity *next;
};
typedef struct
{
    int x;
    int y;
    char label[10];
} Option;
Option options[NUM_OPTIONS] = {
    {400, 300, "START"},
    {400, 316, "OPTIONS"},
    {400, 330, "EXIT"},
};
struct Entity menu_Bg;
struct Entity gfx_Cursor;

void cursorUp();
void cursorDown();
void menuLoop();
void pickStart();
void pickOptions();
void pickExit();




