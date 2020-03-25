#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "cp_functions.h"
using namespace std;

Texture background_texture_menu;
Event eventPage;
Sound end_sound2;
Font big_font2;
FILE *readScore;

char textMenu[4][80], textScore[10][80];

int runningMenuPage;
int runningScorePage;
int currentMenu;
/* 2 = Start game
   3 = See Score
   4 = Shop
   5 = exit

   speical number 1 is menu page
*/

//other variable
int countOutput;

void setQuit(int &runningPage){
   currentMenu = 0;
   runningPage = 0;
}


int initTemplate(){
   end_sound2 = cpLoadSound("theEnd.wav");
   background_texture_menu = cpLoadTexture("background.png");
   big_font2 = cpLoadFont("THSarabun.ttf", 60);
   
   return background_texture_menu && big_font2;
}


int menuPage(int WindowWidth, int WindowHeight){

   initTemplate();
   runningMenuPage = 1;
   currentMenu = 2;

   while(runningMenuPage){
      cpClearScreen();
      cpDrawTexture(255, 255, 255,
                  0, 0, WindowWidth, WindowHeight, background_texture_menu);
       while(cbEventListener(&eventPage)){
         if (eventPage.type == KEYUP){
            if(eventPage.key.keysym.sym == K_SPACE) {
               cpPlaySound(end_sound2);
               runningMenuPage = 0;
            } else if (eventPage.key.keysym.sym == K_DOWN) {
               if( ++currentMenu > 4 ) currentMenu = 2;
            } else if (eventPage.key.keysym.sym == K_UP) {
               if( --currentMenu < 2 ) currentMenu = 4;
            } else if (eventPage.key.keysym.sym == K_ESCAPE) {
               setQuit(runningMenuPage);
            } 
         } else {
            if (eventPage.type == QUIT) {
               setQuit(runningMenuPage);
            }
         }
      }
      
      //write text
      sprintf(textMenu[0], "Start Game");
      sprintf(textMenu[1], "See Record");
      sprintf(textMenu[2], "exit");
      cpDrawText(255, 255, (currentMenu==2?0:250), 400, 350, textMenu[0], big_font2, 1);
      cpDrawText(255, 255, (currentMenu==3?0:250), 400, 350+40, textMenu[1], big_font2, 1); 
      cpDrawText(255, 255, (currentMenu==4?0:250), 400, 350+80, textMenu[2], big_font2, 1);
      cpSwapBuffers(); 
      cpDelay(10);
   } 

   return currentMenu;
}

int scorePage(int WindowWidth,int  WindowHeight){

   initTemplate();
   runningScorePage = 1;
   readScore = fopen(FILE_SCORE, "r");

   for(int count=0, countIndex; count<10; count++) {
      fgets(textScore[count], 80, readScore);
   }
   fclose(readScore);


   while(runningScorePage){
      cpClearScreen();
      cpDrawTexture(255, 255, 255,
                  0, 0, WindowWidth, WindowHeight, background_texture_menu);
      
      cpDrawText(255, 255, 255, 400, 120, "คะแนนสูงสุด", big_font2, 1);
      for(int count=0; count<10; count++) {
         cpDrawText(255, 255, 255, 400, 160+40*count, textScore[count], big_font2, 1);
      }
      cpSwapBuffers();

      while(cbEventListener(&eventPage)){
         if (eventPage.type == KEYUP){
            if (eventPage.key.keysym.sym == K_ESCAPE) {
               setQuit(runningScorePage);
            } 
         } else {
            if (eventPage.type == QUIT) {
               return 0; 
            }
         }
      }
      cpDelay(10);
   } 

   return 1;
}