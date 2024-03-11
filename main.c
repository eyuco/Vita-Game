/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "actors.h"
#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <stdbool.h>
#include <vita2d.h>
#include "level.h"

#define PLAYER_SPEED          4
#define PLAYER_BULLET_SPEED   16
#define WALKING_ANIMATION_FRAMES  2
/*
 * Symbol of the image.png file
 * use entity struct
 * box texture:DONE
 * player collision with box:DONE
 * check malloc
 * draw box before target
 * animate sprites:WIP want to add more
 * Maybe put blocks in array and draw from the tileset
 * draw level://follow sdl2 book for guidance. iwant to learn more about drawing tilemaps
 */
extern unsigned char _binary_image_png_start;
extern unsigned char _binary_player1_png_start;
extern unsigned char _binary_bullet_png_start;
extern unsigned char _binary_zombie_png_start;
extern unsigned char _binary_image1_png_start;
extern unsigned char _binary_image2_png_start;
extern unsigned char _binary_target_png_start;
extern unsigned char _binary_tileset_png_start;

bool isFacingUp = false;
bool isFacingRight = false;
bool isFacingLeft = false;
bool isFacingDown = false;
int isShooting = 0;


void setTexturePosition(vita2d_texture *sprite);
void initPlayer();
void fireBullet();
void updateTexture();
 int vita2d_texture_set_width(const vita2d_texture *texture,int w,int h);
struct Bullets {   // Structure declaration
  float bulPosX;           // Member (int variable)
  float bulletSpeed;       // Member (char variable)
};
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
bool check_collision( struct Entity A, struct Entity B );
float getTexturePosition(vita2d_texture *entity);
struct Entity player;
struct Entity box;
struct Entity collider;
struct Entity goal;
struct Entity gSpriteClips[WALKING_ANIMATION_FRAMES];
struct Entity tile;
int frame = 0;

float bulPosX;
	float bulPosY;
int main()
{
	SceCtrlData pad;
	vita2d_pgf *pgf;
	vita2d_pvf *pvf;
	vita2d_texture *image;
	vita2d_texture *bullet[10];
	vita2d_texture *zombie;
	vita2d_texture *target;
    vita2d_texture *tileset;
	//vita2d_texture_vertex pos;
	float rad = 0.0f;
//struct Entity currentClip = gSpriteClips[frame/4];
	//pos.x = 0.0f;
	player.dx = 5;//45
     player.x = 940/2;
	 player.y = 544/2;
	 player.w = 27.0f;
	 player.h = 33.0f;
	 box.y = 920/2;
	 box.x = 544/2;
     box.w = 46;
	 box.h = 45;

	 goal.x = 940/2;
	 goal.y = 544/2;
	 goal.w = 46;
	 goal.h = 45;

	 tile.x = 1.0f;
	 tile.y = 0.0f;
	 tile.w = 35.0f;
	 tile.h = 37.0f;

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
	zombie = vita2d_load_PNG_buffer(&_binary_zombie_png_start);
	bullet[0] = vita2d_load_PNG_buffer(&_binary_bullet_png_start);
   player.image[0] = vita2d_load_PNG_buffer(&_binary_image_png_start);
   player.image[1] = vita2d_load_PNG_buffer(&_binary_image1_png_start);
   player.image[2] = vita2d_load_PNG_buffer(&_binary_image2_png_start);
   target = vita2d_load_PNG_buffer(&_binary_target_png_start);
   tileset = vita2d_load_PNG_buffer(&_binary_tileset_png_start);

  // int w = vita2d_texture_get_width(player.image[0]);


	memset(&pad, 0, sizeof(pad));

	while (1) {
		sceCtrlPeekBufferPositive(0, &pad, 1);


       /*if(check_collision(player,box) )//player.x < 0 || player.x > 944
       {
        // player.x = player.x;
		   //move box
            //player.x += player.dx;
			box.x += 5;
        }*/
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



        //
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
			player.x += 5;
			bulPosX += 5;
			//run moving sprite
			if(check_collision(player,box))
       {

			box.x += 5;
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

			player.x -= 5;
			bulPosX -= 5;
			//run moving sprite
			if(check_collision(player,box) )//player.x < 0 || player.x > 944
       {
        // player.x = player.x;
		   //move box
            //player.x += player.dx;
			box.x -= 5;
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
            player.y -= 5;
			//bulPosY -= 10;
			bulPosY = player.y;
			if(check_collision(player,box) )//player.x < 0 || player.x > 944
       {
        // player.x = player.x;
		   //move box
            //player.x += player.dx;
			box.y -= 5;
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


			player.y += 5;
			//bulPosY += 10;
			bulPosY = player.y;


			if(check_collision(player,box) )//player.x < 0 || player.x > 944
       {
        // player.x = player.x;
		   //move box
            //player.x += player.dx;
			box.y += 5;
        }




			//vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);
			//break;
		}
        if (pad.buttons & SCE_CTRL_CROSS)
		{
            fireBullet();

           /* if(isFacingUp == true)
				{
					bulPosY -= 100;
				}
			if(isFacingRight == true)
			{
			bulPosX += 100;
			}
            if(isFacingDown == true)
				{
					bulPosY += 100;
				}
			if(isFacingLeft == true)
			{
				bulPosX -= 100;
			}*/
			isShooting = 1;
			//vita2d_draw_texture_rotate(image, 940/2, 544/2, rad);
			//break;}
		}
		if (pad.buttons & SCE_CTRL_START)
		{  // pos.x += 10;
			//break;
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

        //vita2d_draw_texture(bullet[0], bulPosX, bulPosY);
        vita2d_draw_texture(zombie, box.x, box.y);
		vita2d_draw_texture(target, goal.x, goal.y);
		//vita2d_draw_texture(image, x, y);
         //vita2d_create_empty_texture_rendertarget(gSpriteClips[frame].w, gSpriteClips[frame].h, SCE_GXM_TEXTURE_FORMAT_U8_000R);

         vita2d_draw_texture_part(tileset, tile.x , tile.y , currentTile.x, currentTile.y, currentTile.w,currentTile.h);
         vita2d_draw_texture_part(player.image[0], player.x , player.y , gSpriteClips[frame].x, gSpriteClips[frame].y, gSpriteClips[frame].w,gSpriteClips[frame].h);

		 ++frame;
             //sceKernelDelayThread(100000);//delay for one second 100000

                //Cycle animation need to slow it down
                if( frame / 1 >= WALKING_ANIMATION_FRAMES )
                {

					frame = 0;//0
                }

                //sceKernelDelayThread(100000);//delay for one second 100000
         //initPlayer();
		//vita2d_draw_texture(bullet, bulPosX, bulPosY);
		/*vita2d_draw_line(500, 30, 800, 300, RGBA8(255, 0, 255, 255));

		vita2d_pgf_draw_text(pgf, 700, 30, RGBA8(0,255,0,255), 1.0f, "PGF Font sample!");

		vita2d_pvf_draw_text(pvf, 700, 80, RGBA8(0,255,0,255), 1.0f, "PVF Font sample!");

		size_t n_vertices = 69;
		vita2d_color_vertex *vertices = (vita2d_color_vertex *)vita2d_pool_memalign(
			n_vertices * sizeof(vita2d_color_vertex),
			sizeof(vita2d_color_vertex));

		for (int i=0; i<n_vertices; ++i) {
			vertices[i].x = 830.f + 100.f * fabsf(sinf(i*10.f+rad)) * sinf(i*0.1f+rad*0.4f);
			vertices[i].y = 420.f + 100.f * fabsf(sinf(i*10.f+rad)) * cosf(i*0.1f+rad*0.4f);
			vertices[i].z = 0.5f;
			vertices[i].color = RGBA8(0xff-i*2, i*3, 0x8a-2*i, 0x80);
		}

		vita2d_draw_array(SCE_GXM_PRIMITIVE_TRIANGLE_STRIP, vertices, n_vertices);

		size_t nslices = 50;
		size_t n_tvertices = 6 * nslices;
		vita2d_texture_vertex *tvertices = (vita2d_texture_vertex *)vita2d_pool_memalign(
			n_tvertices * sizeof(vita2d_texture_vertex),
			sizeof(vita2d_texture_vertex));

		for (int slice=0; slice<nslices; slice++) {
			float a = (float)slice/(float)nslices;
			float b = (float)(slice+1)/(float)nslices;

			vita2d_texture_vertex *v = &tvertices[slice*6];
			(v++)->u = a; (v++)->u = a; (v++)->u = b;
			(v++)->u = a; (v++)->u = b; (v++)->u = b;
		}

		for (int i=0; i<n_tvertices; ++i) {
			tvertices[i].v = i % 2;
			tvertices[i].x = 720.f + 200.f * tvertices[i].u;
			tvertices[i].y = 100.f + 200.f * tvertices[i].v + 10.f * sinf(tvertices[i].u*(3.f+40.f*fabsf(sinf(rad*0.1f)))+rad);
			tvertices[i].z = 0.5f;
		}

		//vita2d_draw_array_textured(image, SCE_GXM_PRIMITIVE_TRIANGLES, tvertices, n_tvertices, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));

		vita2d_draw_rectangle(40, 40, 100, 100, RGBA8(128, 64, 192, 255));
		vita2d_set_blend_mode_add(1);
		vita2d_draw_rectangle(40, 60, 200, 60, RGBA8(0, 100, 0, 128));
		vita2d_set_blend_mode_add(0);
         */
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
	vita2d_fini();
	vita2d_free_texture(player.image[0]);
	vita2d_free_texture(bullet[0]);
	vita2d_free_texture(zombie);
	vita2d_free_pgf(pgf);
	vita2d_free_pvf(pvf);

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
