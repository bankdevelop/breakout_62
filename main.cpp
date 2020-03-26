#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
//c library for use together with cp_function
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "cp_functions.h"
#include "page_functions.h"

#define WindowTitle "Breakout 62"
#define WindowWidth 800
#define WindowHeight 700
#define MaxLevel 5


int level; //variable for check my current level
int wantQuit; // use case type key QUIT -- in high function can't out loop just break // so we use variable to quit

int status; // check page we at

bool running; // check game running

int amountHitLevel; // use for check if amountHitLevel = nHit so brick run out; can go to nextLevel
int randomValue;

//Life
int life;

//inital speed ball and paddle
float BALL_VEL_Y, PADDLE_SPEED_MOVE, PADDLE_SPEED_VECTOR;

Sound hit_paddle_sound, hit_brick_sound;
Sound hit_top_sound, end_sound;
Texture paddle_texture, ball_texture;
Texture brick_texture_1, brick_texture_2, 
         brick_texture_type_1, brick_texture_type_2,
         background_texture;
Font big_font, small_font;
Event event;
FILE *readScore_main;
time_t curtime;

//---------------------------------------Class---------------------------------------//

// Structure for storing info for objects, i.e. Paddle, Brick, Ball.
class Object
{
public:
   float pos_x, pos_y;
   float vel_x, vel_y;
   float width, height;
   int active;

   //constructor on Object
   Object(){
      Object(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false);
   }
   Object(float in_pos_x, float in_pos_y, float in_vel_x, float in_vel_y, 
          float in_width, float in_height, int in_active)
    : pos_x(in_pos_x), pos_y(in_pos_y), vel_x(in_vel_x), vel_y(in_vel_y), 
      width(in_width), height(in_height), active(in_active) {}

   //method
   int collide(Object b)
   {
      if (this->pos_x + this->width < b.pos_x || b.pos_x + b.width < this->pos_x ||
         this->pos_y + this->height < b.pos_y || b.pos_y + b.height < this->pos_y)
         return False;
      else
         return True;
   }
   void move(float x, float y){
      ;
   }

};

class Brick: public Object
{
public:
   int type = 0;
   Brick(){
      Object(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, false);
   }
   Brick(float in_pos_x, float in_pos_y, float in_vel_x, float in_vel_y, 
         float in_width, float in_height, int in_active, int type) {
      Object(in_pos_x, in_pos_y, in_vel_x, in_vel_y, in_width, in_height, in_active);
      this->type = type;
   }

   bool collide(Object b)
   {
      if( this->active > 0){
         if (this->pos_x + this->width < b.pos_x || b.pos_x + b.width < this->pos_x ||
            this->pos_y + this->height < b.pos_y || b.pos_y + b.height < this->pos_y){
            return false;
         }else{
            this->active--;
            return true;
         }
      }
   }
};


//---------------------------------------functions---------------------------------------//

// Initial routine to load sounds, textures, and fonts.
int game_init()
{
   hit_paddle_sound = cpLoadSound("asset/sound/hitDown.wav");
   hit_brick_sound = cpLoadSound("asset/sound/hitUp.wav");
   hit_top_sound = cpLoadSound("asset/sound/hitTop.wav");
   end_sound = cpLoadSound("asset/sound/theEnd.wav");

   paddle_texture = cpLoadTexture("asset/img/paddle.png");
   ball_texture = cpLoadTexture("asset/img/ball.png");
   brick_texture_1 = cpLoadTexture("asset/img/brick0.png");
   brick_texture_2 = cpLoadTexture("asset/img/brick.png");
   brick_texture_type_1 = cpLoadTexture("asset/img/brick1.png");
   brick_texture_type_2 = cpLoadTexture("asset/img/brick2.png");
   background_texture = cpLoadTexture("asset/img/background.png");

   big_font = cpLoadFont("asset/font/THSarabun.ttf", 60);
   small_font = cpLoadFont("asset/font/THSarabun.ttf", 30);

   return hit_paddle_sound && hit_brick_sound && hit_top_sound && end_sound &&
      paddle_texture && ball_texture && brick_texture_1 && brick_texture_type_1 
      && brick_texture_type_2 && brick_texture_2 && background_texture &&
      big_font && small_font;
}

// Special Objects
Texture ghost_texture;
Object ghost = {0, WindowHeight / 2, 0, 0, 50, 50, True};
int ready_to_swap=1;
pthread_t ghost_thread;

void *ghostBot(void *ptr)
{  double timestep=0;
   float vel_x=5;

   while (True) {
      if (!ready_to_swap) {
         usleep(1000);
         continue;
      }
      if (ghost.pos_x < 0) {
         ghost.pos_x = 0;
         vel_x *= -1.;
      }
      else if (ghost.pos_x > WindowWidth - ghost.width) {
         ghost.pos_x = WindowWidth - ghost.width;
         vel_x *= -1.;
      }
      ghost.pos_x += vel_x;
      ghost.pos_y = WindowHeight/2 + 50*sin(8*M_PI*timestep/(WindowWidth - ghost.width));
      timestep += fabs(vel_x);
      ready_to_swap = 0;
   }
}

int writeScore(int score){
   char weekday[10][10], month[10][10], day[10][10], times[10][10], year[10][10], msg[10][80];
   int scoreGame[10];
   time(&curtime);
   readScore_main = fopen(FILE_SCORE, "r");

   for(int count = 0; count<10; count++){
      fscanf(readScore_main, "%s %s %s %s %s -> %d", &weekday[count], &month[count], &day[count], // 25/03/2020 bug start at fscanf 
                                                &times[count], &year[count], &scoreGame[count]);
      if(weekday[count]==NULL) break;
      strcat(msg[count], weekday[count]);
      strcat(msg[count], " ");
      strcat(msg[count], month[count]);
      strcat(msg[count], " ");
      strcat(msg[count], day[count]);
      strcat(msg[count], " ");
      strcat(msg[count], times[count]);
      strcat(msg[count], " ");
      strcat(msg[count], year[count]);
   }
   
   //Sort Score, if Score have more than any tenth.
   readScore_main = fopen(FILE_SCORE, "w");
   char str_score[80];
   sprintf(str_score, "%d", score);
   for(int count = 0, count2; count < 10 && msg[count]!=NULL ; count++){
      if(score > scoreGame[count]) {
         for(count2 = 10-1; count2>=count; count2--){
            scoreGame[count2] = scoreGame[count2-1];
            strncpy(msg[count2], msg[count2-1], 24);
         }
         scoreGame[count] = score;
         strncpy(msg[count], ctime(&curtime), 24);
         break;
      }
   }

   //write data back to file
   for(int count = 0; count < 10 && msg[count]!=NULL ; count++){
      fprintf(readScore_main ,"%s -> %d\n", msg[count], scoreGame[count]);
   }

   fclose(readScore_main);
   return 0; 
}

void waitQuitEvent()
{
   while (True) {
      cbEventListener(&event);
      if (event.type == QUIT) {
         wantQuit = True;
         running = False;
         break;
      } else if (event.type == KEYUP && event.key.keysym.sym == K_SPACE || 
               event.type == KEYUP && event.key.keysym.sym == K_ESCAPE){
         running = False;
         break;
      }
   }
}

void nextLevel(int &n_hits, int goal, int score)
{
   if(n_hits >= goal){
      cpPlaySound(end_sound);
      n_hits = 0;
      level++;
      //running = false for resetgame
      running=False;
   }
   //if go to MaxLevel, End while loop level
   if(level > MaxLevel) {
      waitQuitEvent();
   }
}

void forceNextLevel(int &n_hits){
   cpPlaySound(end_sound);
   n_hits = 0;
   level++;
   //running = false for resetgame
   running=False;
   if(level > MaxLevel) waitQuitEvent();
}

//checking game ending
void endGame(Object &ball, int &n_hits, int n_bricks, int h_bricks, int score)
{
   if (ball.pos_y + ball.width > WindowHeight) {
      if(--life==0){
         cpPlaySound(end_sound);
         cpDrawText(255, 255, 0, 400, 350, "เกมจบแล้ว", big_font, 1);
         writeScore(score);
         cpSwapBuffers();
         waitQuitEvent();
      }else{
         char msg[80], msg2[80];
         //announce remain life
         cpPlaySound(end_sound);
         sprintf(msg2, "กด spacebar เพื่อเล่นต่อ");
         cpDrawText(255, 255, 255, 400, 350+40, msg2, big_font, 1);
         cpSwapBuffers();
         while (True) {
            cbEventListener(&event);
            if(event.key.keysym.sym == K_SPACE) break;
         }
         ball.pos_x = WindowWidth / 2 - 12;
         ball.pos_y = 350;
         ball.vel_x = 0;
         ball.vel_y = -BALL_VEL_Y;
         return;
      }
   }
}

void paddleCheckEvent(Object &paddle, int &n_hits)
{
   //check input keyboard
   while (cbEventListener(&event)) {
      if (event.type == QUIT) {
         wantQuit = True;
         running = False;
         break;
      }else if(event.type == KEYUP && event.key.keysym.sym == K_ESCAPE){
         running = False;
         break;
      }

      if (event.type == KEYDOWN) {
         if (event.key.keysym.sym == K_LEFT)
            paddle.vel_x -= PADDLE_SPEED_VECTOR;
         if (event.key.keysym.sym == K_RIGHT)
            paddle.vel_x += PADDLE_SPEED_VECTOR;
      }
      else if (event.type == KEYUP) {
         if (event.key.keysym.sym == K_LEFT)
            paddle.vel_x = 0;
         if (event.key.keysym.sym == K_RIGHT)
            paddle.vel_x = 0;
         if (event.key.keysym.sym == K_KP_1)
            forceNextLevel(n_hits);
      }
   }
   //move paddle
   paddle.pos_x += PADDLE_SPEED_MOVE*(paddle.vel_x==0?0:(paddle.vel_x>0?1:-1));

   //check paddle not out of edge
   if (paddle.pos_x < 0)
      paddle.pos_x = 0;
   if (paddle.pos_x + paddle.width > WindowWidth)
      paddle.pos_x = WindowWidth - paddle.width;
}

void initialBrick(Brick &bricks, double x, double y, 
                  int bricks_width, int bricks_height, int active, int type)
{
   bricks.pos_x = x;
   bricks.pos_y = y;
   bricks.width = bricks_width;
   bricks.height = bricks_height;
   bricks.type = type;
   bricks.active = (bricks.type>=1)?1:active;
}

int runGame()
{
   int h_bricks = 8, n_bricks = 15, n_hits = 0, score = 0;
   amountHitLevel = 0;

   //inital start value
   level = 1;
   wantQuit = 0;
   life = 3;
   BALL_VEL_Y = (-5*level);
   PADDLE_SPEED_MOVE = 7*level;
   PADDLE_SPEED_VECTOR = 1*level;

   char msg[80];
   Brick bricks[h_bricks][n_bricks];
   Object ball = {WindowWidth / 2 - 12, 350, 0, BALL_VEL_Y, 24, 24, True};
   Object paddle = {WindowWidth / 2 - 62, WindowHeight - 50, 0, 0, 124, 18, True};

   if (!game_init()) {
      fprintf(stderr, "Game initialization failed!\n");
      exit(1);
   }

   ghost_texture = cpLoadTexture("asset/img/ghost.png");
   pthread_create(&ghost_thread, NULL, ghostBot, (void *)NULL);

   int lap_current = 1;
   //While Loop level -> Check in case we not pass to next level but lose, lap_current and level will not equal
   while(lap_current == level){

      /* resetting value initial */ BALL_VEL_Y = (-5*level), PADDLE_SPEED_MOVE = 7*level, PADDLE_SPEED_VECTOR = 1*level;
      //create new bricks
      srand(time(NULL));
      for(int h = 0, y = 80; h<h_bricks; h++){  
         int x = -10;
         int bricks_width = 55;
         int bricks_height = 18;
         
         for (int n = 0; n < n_bricks; n++) {
               randomValue = (10 + rand( ) % 46);
               initialBrick(bricks[h][n], x, y, bricks_width, bricks_height, (randomValue%2)+1, 
                           (level==2)?randomValue%2:((level>=3)?randomValue%3:0));
               amountHitLevel+=bricks[h][n].active;
               x += bricks[h][n].width;
         }
         y += bricks_height;
      }

      running = True;
      //Loop in each Level
      while (running) {
         cpClearScreen();
         cpDrawTexture(255, 255, 255,
                     0, 0, WindowWidth, WindowHeight, background_texture);
         cpDrawTexture(255, 255, 255,
                     paddle.pos_x, paddle.pos_y, paddle.width, paddle.height, paddle_texture);
         cpDrawTexture(255, 255, 255,
                     ball.pos_x, ball.pos_y, ball.width, ball.height, ball_texture);
         cpDrawTexture(255, 255, 255,
                     ghost.pos_x, ghost.pos_y, ghost.width, ghost.height, ghost_texture);
         ready_to_swap = 1;

         //draw brick Texture
         for(int h = 0; h<h_bricks; h++){
            for (int n = 0; n < n_bricks; n++) {
               if (bricks[h][n].active)
                  cpDrawTexture(255, 255, 255,
                              bricks[h][n].pos_x, bricks[h][n].pos_y, bricks[h][n].width, bricks[h][n].height,
                              (bricks[h][n].type==1)?brick_texture_type_1:
                                 (bricks[h][n].type==2)?brick_texture_type_2:
                                    (bricks[h][n].active==2?brick_texture_2:brick_texture_1));
            }
         }

         //announce point & level
         sprintf(msg, "คะแนน %d | Level %d | Life %d", score, level, life);
         cpDrawText(255, 255, 255, 3, 3, msg, small_font, 0);
         
         //check END game
         endGame(ball, n_hits, n_bricks, h_bricks, score);
         cpSwapBuffers();

         //paddle check event
         paddleCheckEvent(paddle, n_hits);

         //move ball object
         ball.pos_x += ball.vel_x;
         ball.pos_y += ball.vel_y;

         //check ball hit edge
         if (ball.pos_x < 0 || ball.pos_x + ball.width > WindowWidth)
            ball.vel_x = -ball.vel_x;
         if (ball.pos_y < 0) {
            cpPlaySound(hit_top_sound);
            ball.vel_y = -ball.vel_y;
         }

         //check ball hit brinks
         for(int h = 0; h<h_bricks; h++){
            for (int n = 0; n < n_bricks; n++) {
               if (bricks[h][n].active && ball.collide(bricks[h][n])) {
                  cpPlaySound(hit_brick_sound);
                  ball.vel_y = -ball.vel_y;
                  bricks[h][n].active--;
                  n_hits++;
                  if(bricks[h][n].type == 1) score += 30;
                  else if(bricks[h][n].type == 2) {
                     score+=300;
                     if(h-1>=0) {
                        (bricks[h-1][n].active>=1)?(bricks[h-1][n].active--):(bricks[h-1][n].active=0);
                        n_hits++;
                     }
                     if(h+1<h_bricks) {
                        (bricks[h+1][n].active>=1)?(bricks[h+1][n].active--):(bricks[h+1][n].active=0);
                        n_hits++;
                     }
                     if(n+1<n_bricks) {
                        (bricks[h][n+1].active>=1)?(bricks[h][n+1].active--):(bricks[h][n+1].active=0);
                        n_hits++;
                     }
                     if(n-1>=0) {
                        (bricks[h][n-1].active>=1)?(bricks[h][n-1].active--):(bricks[h][n-1].active=0);
                        n_hits++;
                     }
                  }else score+=10;
                  nextLevel(n_hits, amountHitLevel, score);
                  break;
               }
            }
         }

         //check ball hit paddle
         if (ball.collide(paddle)) {
            cpPlaySound(hit_paddle_sound);
            ball.vel_x = ball.vel_x+paddle.vel_x;
            ball.vel_y = -ball.vel_y;
         }

         cpDelay(10);
      }
      lap_current++;
   }
   return !wantQuit;
}

int main(int argc, char *args[])
{
   if (!cpInit(WindowTitle, WindowWidth, WindowHeight)) {
      fprintf(stderr, "Window initialization failed!\n");
      exit(1);
   }

   status = 1; // 1 is menu page

   while(True){
      if(status == 1) {
         status = menuPage(WindowWidth, WindowHeight);
         if( status == 2 ) 
            status = runGame();
      }else if(status == 3) 
         status = scorePage(WindowWidth, WindowHeight);
      else break;
   }
   
   cpCleanUp(); 
   return 0; 
}

