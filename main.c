/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//#include "actors.h"
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <stdbool.h>
#include <vita2d.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include "gMenu.h"
#include "util.h"
#include "write_read.h"
#include <vorbis/vorbisfile.h>
//#include "vitaAudio/vitaAudio.h"
#include <psp2/audioout.h>
#include <psp2/audiodec.h> 
#include <mad.h>
#include "Audio.h"
//#include <psp2/kernel/threadmgr.h>
//#include <psp2/kernel/processmgr.h>
//#include "debugScreen.h"

#define PLAYER_SPEED          4
#define PLAYER_BULLET_SPEED   16
//#define WALKING_ANIMATION_FRAMES  2
#define DATA_LEN        2048
#define SCE_AUDIODEC_MP3_MAX_STREAMS 8
#define MAX(a,b)        ((a) > (b) ? (a) : (b))
#define BUFFER_SIZE   32768

typedef double (*wav_gen)(double);

void wave_set(int16_t*buffer, size_t size,  wav_gen generator){
	for (size_t smpl = 0; smpl < size; ++smpl)
		buffer[smpl] = 0x7FFF*generator((float)smpl/(float)size);
}

static int check_output (const float * data_in, unsigned len, float allowable);

/*
 * Symbol of the image.png file
 * use entity struct
 * box texture:DONE
 * player collision with box:DONE
 * check malloc
 * draw box before target
 * animate sprites:DONE
 * Maybe put blocks in array and draw from the tileset
 * draw grid:DONE fill it with tileset blocks
 * draw level://follow sdl2 book for guidance. iwant to learn more about drawing tilemaps
 * draw collision boxes around level area:DONE
 * draw box in level area:DONE
 * add target boxes: Done
 * draw start menu:DONElook at lazyfoo game states tut
 * organize code
 *need to center menu text:DONE
 *Draw level select screen
 */
extern unsigned char _binary_image_png_start;
extern unsigned char _binary_player1_png_start;
extern unsigned char _binary_bullet_png_start;
extern unsigned char _binary_zombie_png_start;
extern unsigned char _binary_image1_png_start;
extern unsigned char _binary_image2_png_start;
extern unsigned char _binary_target_png_start;
extern unsigned char _binary_tileset_png_start;
extern unsigned char _binary_bg_png_start;
extern unsigned char _binary_blocks_png_start;

AudioHandler _audio;
bool isFacingUp = false;
bool isFacingRight = false;
bool isFacingLeft = false;
bool isFacingDown = false;
bool state_Menu = false;
bool state_Game = false;


bool paused;

enum GAME_STATE
{
    STATE_MENU,
    STATE_PLAY
};
enum GAME_STATE currentState;


//int currentIndex = 0;
void updateCursorPosition();
void joyEventHandler(int joy, int changed, int state);
//void moveUp();
void moveDown();
void Select(int Option);

void setTexturePosition(vita2d_texture *sprite);
void initPlayer();
void fireBullet();
void updateTexture();
 int vita2d_texture_set_width(const vita2d_texture *texture,int w,int h);
 void drawLevelLayer();
struct Bullets {   // Structure declaration
  float bulPosX;           // Member (int variable)
  float bulletSpeed;       // Member (char variable)
};

bool check_collision( struct Entity A, struct Entity B );
float getTexturePosition(vita2d_texture *entity);
struct Entity player;
struct Entity box;
struct Entity collider;
struct Entity goal;
struct Entity gSpriteClips[WALKING_ANIMATION_FRAMES];
struct Entity tile;
struct Entity columns;
//struct Entity gfx_Cursor;
//struct Entity menu_Bg;
int frame = 0;
//vita2d_texture *cursor;
//vita2d_texture *menu_BG;
float bulPosX;
	float bulPosY;
	vita2d_pgf *pgf;
	char pcmout[4096];
int main(int argc, char** argv)
{
	SceCtrlData pad;
	//vita2d_pgf *pgf;
	vita2d_pvf *pvf;
	vita2d_texture *image;
	vita2d_texture *bullet[10];
	vita2d_texture *zombie[10];
	vita2d_texture *target[10];
    vita2d_texture *tileset[27];
	vita2d_texture *bg;
	vita2d_texture *blocks[10];
	//vita2d_texture *cursor;
  //test
#define OGG_IMPL
#define VORBIS_IMPL

//test
	menuState = true;
	currentState = STATE_MENU;

	//vita2d_texture_vertex pos;
	float rad = 0.0f;
	int grid_cell_size = 36;
    int grid_width = 960;
    int grid_height = 544;
	 int window_width = (grid_width * grid_cell_size) + 1;
    int window_height = (grid_height * grid_cell_size) + 1;
struct grid_cursor {
      int  x ;
       int y ;
       int w;
       int h;
    };
         struct grid_cursor grid;
	    grid.x = (grid_width - 1) / 2 * grid_cell_size;
        grid.y = (grid_height - 1) / 2 * grid_cell_size;
        grid.w = grid_cell_size;
        grid.h = grid_cell_size;
		//struct grid_cursor_ghost = {grid.x, grid.y, grid_cell_size,
     
//struct Entity currentClip = gSpriteClips[frame/4];
	//pos.x = 0.0f;
        int column[30];
	player.dx = 5;//45
     player.x = 940/2 + grid_cell_size;
	 player.y = 544/2 + grid_cell_size;
	 player.w = 27.0f;
	 player.h = 33.0f;
	 box.y = 500; //+ grid_cell_size;//920/2;
	 box.x = 900;// + grid_cell_size;//544/2;
	 //box.x = column[17];
	 //box.y = column[7];
     box.w = 46;
	 box.h = 45;
	 gfx_Cursor.x = options[currentIndex].x-15;
     gfx_Cursor.y = options[currentIndex].y-10;
    menu_Bg.w = 740.0f;
    menu_Bg.h = 740.0f;


	 goal.x = 940/2 + grid_cell_size;
	 goal.y = 544/2 + grid_cell_size;
	 goal.w = 46;
	 goal.h = 45;

	 tile.x = 1.0f;
	 tile.y = 0.0f;
	 tile.w = 35.0f;
	 tile.h = 37.0f;

	// columns.x = 1.0f;
	 //columns.y = 0.0f;
	 columns.w = 35.0f;
	 columns.h = 37.0f;



	 /*
	 gSpriteClips[0].x = 0.0f;
	 gSpriteClips[0].y = 0.0f;
	 gSpriteClips[0].w = 27.0f;
	 gSpriteClips[0].h = 33.0f;
*/

     //replace this with isFacingDown = true;
     /*gSpriteClips[0].x = 35.0f;//1
	 gSpriteClips[0].y = 0.0f;
	 gSpriteClips[0].w = 27.0f;
	 gSpriteClips[0].h = 33.0f;

	 gSpriteClips[1].x = 66.0f;//2
	 gSpriteClips[1].y = 0.0f;
	 gSpriteClips[1].w = 27.0f;
	 gSpriteClips[1].h = 33.0f;
*/
	 isFacingDown = true;
struct Entity currentClip = gSpriteClips[frame/4];
struct Entity currentTile = tile;
    float speed = 10.3f;


	vita2d_init();

    

	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

	InitializeAudio(&_audio);
 
	//gen=gen_tri;
//LoadOgg(&_audio, "Sounds/music.ogg", AUDIO_OUT_BGM,0);
  LoadWav(&_audio, "Sounds/music.wav", AUDIO_OUT_MAIN,0);


	pgf = vita2d_load_default_pgf();
	pvf = vita2d_load_default_pvf();
 bulPosX = 940/2;
 bulPosY = 544/2;
float enemyPosX = 950/2;
float enemyPosY = 544/2;


	/*
	 * Load the statically compiled image.png file.
	 */
	//image = vita2d_load_PNG_buffer(&_binary_image_png_start);
	zombie[0] = vita2d_load_PNG_buffer(&_binary_zombie_png_start);
	zombie[1] = vita2d_load_PNG_buffer(&_binary_zombie_png_start);
	bullet[0] = vita2d_load_PNG_buffer(&_binary_bullet_png_start);
   player.image[0] = vita2d_load_PNG_buffer(&_binary_image_png_start);
   player.image[1] = vita2d_load_PNG_buffer(&_binary_image1_png_start);
   player.image[2] = vita2d_load_PNG_buffer(&_binary_image2_png_start);
   target[0] = vita2d_load_PNG_buffer(&_binary_target_png_start);
   target[1] = vita2d_load_PNG_buffer(&_binary_target_png_start);
   //blocks = vita2d_load_PNG_buffer(&_binary_blocks_png_start);
   blocks[0] = vita2d_load_PNG_buffer(&_binary_blocks_png_start);
   blocks[1] = vita2d_load_PNG_buffer(&_binary_blocks_png_start);
   blocks[2] = vita2d_load_PNG_buffer(&_binary_blocks_png_start);
   blocks[3] = vita2d_load_PNG_buffer(&_binary_blocks_png_start);
   blocks[4] = vita2d_load_PNG_buffer(&_binary_blocks_png_start);
   blocks[5] = vita2d_load_PNG_buffer(&_binary_blocks_png_start);
   blocks[6] = vita2d_load_PNG_buffer(&_binary_blocks_png_start);


   tileset[0] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[1] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[2] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[3] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[4] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[5] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[6] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[7] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[8] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[9] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);
   tileset[10] = vita2d_load_PNG_buffer(&_binary_tileset_png_start);

menu_BG = vita2d_load_PNG_buffer(&_binary_menuBG_png_start);
cursor = vita2d_load_PNG_buffer(&_binary_cursor_png_start);
  // int w = vita2d_texture_get_width(player.image[0]);
bg = vita2d_load_PNG_buffer(&_binary_bg_png_start);
//int column[30];
	memset(&pad, 0, sizeof(pad));
unsigned char buf[4096];
	while (1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);

     
        switch(currentState){
            case STATE_MENU:{
               // menuLoop();
                int i = 0;
                //processStateMenu();
                break;
            }
            case STATE_PLAY:{
                //processStatePlay();
                //pickStart();
                break;
            }
        }




	   if(isFacingUp == true)
	   {
		   gSpriteClips[0].x = 35.0f;//1
	       gSpriteClips[0].y = 34.0f;
	       gSpriteClips[0].w = 27.0f;
	       gSpriteClips[0].h = 33.0f;

	       gSpriteClips[1].x = 66.0f;//2
	       gSpriteClips[1].y = 34.0f;
	       gSpriteClips[1].w = 27.0f;
	       gSpriteClips[1].h = 33.0f;

	   }
	   if(isFacingDown == true)
	   {
		   gSpriteClips[0].x = 35.0f;//1
	       gSpriteClips[0].y = 0.0f;
	       gSpriteClips[0].w = 27.0f;
	       gSpriteClips[0].h = 33.0f;

	       gSpriteClips[1].x = 66.0f;//2
	       gSpriteClips[1].y = 0.0f;
	       gSpriteClips[1].w = 27.0f;
	       gSpriteClips[1].h = 33.0f;


	   }
	   if(isFacingRight == true)
	   {
		   gSpriteClips[0].x = 35.0f;//1
	       gSpriteClips[0].y = 66.0f;
	       gSpriteClips[0].w = 27.0f;
	       gSpriteClips[0].h = 33.0f;

	       gSpriteClips[1].x = 65.0f;//2
	       gSpriteClips[1].y = 66.0f;
	       gSpriteClips[1].w = 27.0f;
	       gSpriteClips[1].h = 33.0f;


	   }
      if(isFacingLeft == true)
	   {
		   gSpriteClips[0].x = 35.0f;//1
	       gSpriteClips[0].y = 98.0f;
	       gSpriteClips[0].w = 27.0f;
	       gSpriteClips[0].h = 33.0f;

	       gSpriteClips[1].x = 65.0f;//2
	       gSpriteClips[1].y = 98.0f;
	       gSpriteClips[1].w = 27.0f;
	       gSpriteClips[1].h = 33.0f;


	   }



        //  //box meets target
        if(check_collision(box,goal) )//player.x < 0 || player.x > 944
       {
          break;
        }

		if (pad.buttons & SCE_CTRL_RIGHT)
		{
            isFacingRight = true;
			isFacingUp = false;
			isFacingLeft = false;
			isFacingDown = false;
			player.x += grid_cell_size;
			bulPosX += 5;
			columns.x = column[16];
			//box.x = column[14];
			//box.y = column[7];
			//run moving sprite
			if(check_collision(player,goal))
       {

			goal.x += grid_cell_size;
        }

			//vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);
			//break;
		}
		if (pad.buttons & SCE_CTRL_LEFT)
		{
            isFacingLeft = true;
			isFacingDown = false;
			isFacingRight = false;
			isFacingUp = false;

			player.x -= grid_cell_size;
			bulPosX -= 5;
			columns.x = column[7];
			//run moving sprite
			if(check_collision(player,goal) )//player.x < 0 || player.x > 944
       {
        // player.x = player.x;
		   //move box
            //player.x += player.dx;
			goal.x -= grid_cell_size;
        }


			//vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);
			//break;
		}
		if (pad.buttons & SCE_CTRL_UP)
		{
			isFacingUp = true;
			isFacingRight = false;
			isFacingLeft = false;
			isFacingDown = false;
            player.y -= grid_cell_size;//5;
			columns.y = column[3];
            cursorUp();
			//bulPosY -= 10;
			bulPosY = player.y;
			if(check_collision(player,goal) )//player.x < 0 || player.x > 944
       {
        // player.x = player.x;
		   //move box
            //player.x += player.dx;
			goal.y -= grid_cell_size;
        }


			//vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);
			//break;
		}
		if (pad.buttons & SCE_CTRL_DOWN)
		{
            isFacingDown = true;
			isFacingLeft = false;
			isFacingRight = false;
			isFacingUp = false;


			player.y += grid_cell_size;
			columns.y = column[10];
			//bulPosY += 10;
			bulPosY = player.y;
            moveDown();

			if(check_collision(player,goal) )//player.x < 0 || player.x > 944
       {
        // player.x = player.x;
		   //move box
            //player.x += player.dx;
			goal.y += grid_cell_size;
        }
        
		}
        if (pad.buttons & SCE_CTRL_CROSS)
		{
            //fireBullet();
          
			//vaudio_play_music_ogg( snd_mysound, "resources/sokoban.ogg" );
			PlayAudio(&_audio);
			
			//vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);
			//break;}
		}
       

		if (pad.buttons & SCE_CTRL_START)
		{  // pos.x += 10;
			//break;
			//vita2d_clear_screen();
			//vita2d_draw_texture_tint(menu_BG, 100, 0, RGBA8(0, 0, 0, 0));
			//currentState == STATE_PLAY;
           Select(currentIndex);
             menu_Bg.w = 0.0f;
             menu_Bg.h = 0.0f;
			//vita2d_free_texture(menu_BG);
		}
		//test
        sceKernelDelayThread(120000);
		vita2d_start_drawing();
		vita2d_clear_screen();

        //vita2d_start_drawing();
		//vita2d_draw_rectangle(20, 20, 400, 250, RGBA8(255, 0, 0, 255));
		//vita2d_draw_rectangle(680, 350, 100, 150, RGBA8(0, 0, 255, 255));
		//vita2d_draw_fill_circle(200, 420, 100, RGBA8(0, 255,0 ,255));

		//vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);

        vita2d_draw_texture(bg, 100, 0);
        
        //vita2d_draw_texture(zombie, box.x, box.y);



			// int column[30];
			 column[0] = 0;
			 column[1] = 0 + grid_cell_size;
			 column[2] = column[1] + grid_cell_size;
			 column[3] = column[2] + grid_cell_size;
			 column[4] = column[3] + grid_cell_size;
			 column[5] = column[4] + grid_cell_size;
			 column[6] = column[5] + grid_cell_size;
			 column[7] = column[6] + grid_cell_size;
			 column[8] = column[7] + grid_cell_size;
			 column[9] = column[8] + grid_cell_size;
			 column[10] = column[9] + grid_cell_size;
			 column[11] =  column[10] +grid_cell_size;
			 column[12] = column[11] + grid_cell_size;
			 column[13] = column[12] + grid_cell_size;
			 column[14] = column[13] + grid_cell_size;
			 column[15] = column[14] + grid_cell_size;
			 column[16] = column[15] + grid_cell_size;
			 column[17] = column[16] + grid_cell_size;
			 column[18] = column[17] + grid_cell_size;
			 column[19] = column[18] + grid_cell_size;
			 column[20] = column[19] + grid_cell_size;
			 column[21] = column[20] + grid_cell_size;
			 column[22] = column[21] + grid_cell_size;
			 column[23] = column[22] + grid_cell_size;
			 column[24] = column[23] + grid_cell_size;
			 column[25] = column[24] + grid_cell_size;
			 column[26] = column[25] + grid_cell_size;


         //draw the grid     1
         //draw other objects after tileset
         
         for (int x = 0; x < 1 + grid_width * grid_cell_size;
             x += grid_cell_size) {
            //int y = grid_height += grid_cell_size;

           
           //put tileset in array
			 //adjust these tiles for level layout like add something to x
			 //level 1 should be an easy level
            //this could be the level space and the background can be drawn as normal texture
            // vita2d_draw_texture(tileset[0], x, 0);//x,0
             //vita2d_draw_texture(tileset[0], x, 0 + grid_cell_size);
			 //vita2d_draw_texture(tileset[0], x, column[2]);
			// vita2d_draw_texture(tileset[0], x, column[3]);
            
			 vita2d_draw_texture(tileset[0], x, column[4]); //+x 4

             //vita2d_draw_texture(tileset[1], x, column[5]);
             //with this i can draw the collision boxes anywhere on the level area
			 vita2d_draw_texture(tileset[1], x, column[5]);
			 vita2d_draw_texture(tileset[2], x, column[6]);
			 vita2d_draw_texture(tileset[3], x, column[7]);
			 vita2d_draw_texture(tileset[4], x, column[8]);
			 vita2d_draw_texture(tileset[5], x, column[9]);//point
			 //vita2d_draw_texture(tileset[6], x, column[10]);
             //vita2d_draw_texture(tileset[7], x, column[11]);
			 //vita2d_draw_texture(tileset[8], x, column[9]);//point
			 //vita2d_draw_texture(tileset[9], x, column[10]);
             //vita2d_draw_texture(tileset[10], x, column[11]);
             //vita2d_draw_texture(blocks[0], 1 + grid_cell_size, column[10]);//x 11
			 vita2d_draw_texture(blocks[0], x, column[10]);
			 vita2d_draw_texture(blocks[0], x, column[3]);
             //vita2d_draw_texture(blocks[1], 2 + grid_cell_size, column[11]);
			 columns.x = x;
             //box.x = x;
			 if(isFacingUp){


			 if(check_collision(player,columns))
				 {

                 player.y += grid_cell_size;
				}
			 }


             if(isFacingDown){

             /*if(check_collision(player,box)){
             	player.y -= grid_cell_size;
             }*/
			 if(check_collision(player,columns))
				 {

                 player.y -= grid_cell_size;
				}
			 }

			 //vita2d_draw_texture(zombie, box.x, box.y);
			  vita2d_draw_texture_part(player.image[0], player.x , player.y , gSpriteClips[frame].x, gSpriteClips[frame].y, gSpriteClips[frame].w,gSpriteClips[frame].h);

		 ++frame;
             //sceKernelDelayThread(100000);//delay for one second 100000

                //Cycle animation need to slow it down
                if( frame / 1 >= WALKING_ANIMATION_FRAMES )
                {

					frame = 0;//0
                }

			 vita2d_draw_line(x, 0, x, window_height, RGBA8(255, 0, 255, 255));


        }

           //test            //1
        for (int y = 0; y < 1 + grid_height * grid_cell_size;
             y += grid_cell_size) {
            //SDL_RenderDrawLine(renderer, 0, y, window_width, y);

          //vita2d_draw_texture(tileset, 0, y);//y
			//need to figure out draw second y column
					vita2d_draw_texture(blocks[1], column[7], y);//0 + grid_cell_size
					columns.y = y;
					//box.y = column[8];
					//columns.x = column[8];
          		vita2d_draw_texture(blocks[1], column[16], y);//0 + grid_cell_size

					 if(isFacingLeft){
                      columns.x = column[8];

			 if(check_collision(player,columns))
				 {

                 player.x += grid_cell_size;
				}
			 }

             if(isFacingRight){
                //make this equal to all columns
                
                columns.x = column[15];//15
              
             
                
			 if(check_collision(player,columns))
				 {

                 player.x -= grid_cell_size;
				}
			 }


          vita2d_draw_line(0, y, window_width, y, RGBA8(255, 0, 255, 255));
		}
         
        //topA
		//vita2d_draw_texture(blocks[1], column[15], column[9]);
        
        vita2d_draw_texture(zombie[0], goal.x, goal.y);//column[14], box.y
        vita2d_draw_texture(zombie[1], column[15], column[7]);


        
         vita2d_draw_texture(target[0], column[14], column[9]);//box.x, box.y
        //vita2d_draw_texture(target[1], column[15], column[9]);
		

          //vita2d_draw_texture(cursor,options[currentIndex].x*8-12, options[currentIndex].y*8);
         //clip bg when start is pressed
      vita2d_draw_texture_part(menu_BG, 100, 0, 0.0f, 0.0f, menu_Bg.w,menu_Bg.h);
      //vita2d_draw_texture(menu_BG,100, 0);
		vita2d_draw_texture(cursor,gfx_Cursor.x, gfx_Cursor.y);
      menuLoop();
  
       // vita2d_clear_screen();
		vita2d_end_drawing();
		vita2d_swap_buffers();
        //sceKernelDelayThread(100000);//delay for one second 100000
		//rad += 0.1f;

	}

	/*
	 * vita2d_fini() waits until the GPU has finished rendering,
	 * then we can free the assets freely.
	 */

	StopAudio(&_audio);
	TerminateAudio(&_audio);

	vita2d_fini();
	vita2d_free_texture(player.image[0]);
	vita2d_free_texture(bullet[0]);
	vita2d_free_texture(zombie);
	vita2d_free_texture(tileset[0]);
	vita2d_free_pgf(pgf);
	vita2d_free_pvf(pvf);
    //sceAudioOutReleasePort(port);
	sceKernelExitProcess(0);
	return 0;
}
void initPlayer(){
	 //player.x = 940/2;
	// player.y = 544/2;
	//struct Entity *player;
//player->image = vita2d_load_PNG_buffer(&_binary_image_png_start);
    //int w = vita2d_texture_get_width(player);
	//set animation with this                                     texture possiton
	vita2d_draw_texture_part(player.image[0], player.x, player.y, 0.0f, 0.0f, player.w,player.h);//27.0f 33.0f


    
	//vita2d_draw_texture(player.image[0], player.x, player.y);

//vita2d_create_empty_texture_rendertarget(player.w, player.h, SceGxmTextureFormat format);
//vita2d_free_texture(image);
}
int vita2d_texture_set_width(const vita2d_texture *texture,int w,int h){


}

void fireBullet()
{
	struct Entity *bullet;
	bullet = malloc(sizeof(struct Entity));
	memset(bullet,0,sizeof(struct Entity));
    bulPosX = player.x;
	bulPosY = player.y;
    bulPosY -= 100;

}
float getTexturePosition(vita2d_texture *entity)
{
   struct Entity obj;
   //obj.image = entity;
   //return obj.y;

}
void pickStart(){
    vita2d_start_drawing();
	vita2d_clear_screen();

}
void Select(int Option){
    switch (Option)
    {
    case 0:{
        //start game loop
        //currentState = STATE_PLAY;
        pickStart();
        break;
    }
    case 1:{
        //pickOptions();
        break;
    }
    case 2:{
        //pickExit();
        break;
    }
    
    default:
        break;
    }
}
//test
void menuLoop()
{
    //SPR_reset();

    //SPR_update();
    //PAL_setColors(0, (u16*) palette_black, 64, CPU);
    //JOY_setEventHandler(joyEvent);
    //SYS_setVBlankCallback(vblank);
  //paused = TRUE;
  int i = 0;
for(i; i < NUM_OPTIONS; i++){
    Option o = options[i];
    //VDP_drawText(o.label,o.x,o.y);
    vita2d_pgf_draw_text(pgf, o.x, o.y, RGBA8(0, 255, 0, 255), 1.0f, o.label);//700,30
    
}
   //vita2d_draw_texture(cursor,options[currentIndex].x*8-12, options[currentIndex].y*8);
   //vita2d_draw_texture(menu_BG,940/2, 544/2);
   //vita2d_draw_texture(cursor,options[currentIndex].x*8-12, options[currentIndex].y*8);
   //vita2d_draw_texture(menu_BG,740, 740);

   //cursor = SPR_addSprite(&gfx_cursor, 0, 0, 0);
   //updateCursorPosition();
}

void updateCursorPosition()
{
 
 //SPR_setPosition(cursor, options[currentIndex].x*8-12, options[currentIndex].y*8);

}
void cursorUp()
{     
     if(currentIndex > 0){
        currentIndex--;
        gfx_Cursor.y = options[currentIndex].y-10;
        //updateCursorPosition();
    }
}
void moveDown(){
    
    if(currentIndex < NUM_OPTIONS-1){
        currentIndex++;
       gfx_Cursor.y = options[currentIndex].y-10;
        //updateCursorPosition();
    }
}




//test
bool check_collision( struct Entity A, struct Entity B )
{
//The sides of the rectangles
int leftA, leftB;
int rightA, rightB;
int topA, topB;
int bottomA, bottomB;

//Calculate the sides of rect A
leftA = A.x;//A.x
rightA = A.x + A.w;
topA = A.y;
bottomA = A.y + A.h;

//Calculate the sides of rect B
leftB = B.x;
rightB = B.x + B.w;//B.w
topB = B.y;
bottomB = B.y + B.h;

//If any of the sides from A are outside of B
if( bottomA <= topB )
{
    return false;
}

if( topA >= bottomB )
{
    return false;
}

if( rightA <= leftB )
{
  //  player.x -= player.dx;
	return false;
}

if( leftA >= rightB )//>
{
    return false;
}

//If none of the sides from A are outside B
return true;
}

static int
check_output (const float * data_in, unsigned len, float allowable)
{
  float max_abs = 0.0 ;
  unsigned k ;

  for (k = 0 ; k < len ; k++) {
    float temp = fabs (data_in [k]);
    max_abs = MAX (max_abs, temp);
  }

  if (max_abs < 0.95-allowable) {
    printf ("Error : max_abs (%f) too small.\n", max_abs);
    return 1 ;
  } else if (max_abs > .95+allowable) {
    printf ("Error : max_abs (%f) too big.\n", max_abs);
    return 1 ;
  }

  return 0 ;
}



