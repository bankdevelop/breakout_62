#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <pthread.h>
#include "cp_functions.h"

#define windowTitle "Window title 2"

Texture background_texture_start;
Event eventPage;
Sound end_sound2;
Font big_font2;
int runningStartPage = 1;
int currentMenu = 1;
/* 1 = Start game
   2 = See Score
   3 = Shop
   4 = Setting
*/

int startPage(char *WindowTitle, int WindowWidth, int WindowHeight){
   if (!cpInit(WindowTitle, WindowWidth, WindowHeight)) {
      fprintf(stderr, "Window initialization failed!\n");
      exit(1);
   }

   end_sound2 = cpLoadSound("theEnd.wav");
   big_font2 = cpLoadFont("THSarabun.ttf", 60);
   background_texture_start = cpLoadTexture("background.png");
   char textMenu1[80], textMenu2[80], textMenu3[80], textMenu4[80];

   while(runningStartPage){
      cpClearScreen();
      cpDrawTexture(255, 255, 255,
                  0, 0, WindowWidth, WindowHeight, background_texture_start);
       while(cbEventListener(&eventPage)){
         if (eventPage.type == KEYUP){
            if(eventPage.key.keysym.sym == K_SPACE) {
               cpPlaySound(end_sound2);
               //break;
            } else if (eventPage.key.keysym.sym == K_DOWN) {
               if( ++currentMenu > 4 ) currentMenu=1;
            } else if (eventPage.key.keysym.sym == K_UP) {
               if( --currentMenu < 1 ) currentMenu=4;
            } else if (eventPage.key.keysym.sym == K_ESCAPE) {
               runningStartPage = 0;
            } 
         } else {
            if (eventPage.type == QUIT) {
               runningStartPage = 0;
            }
         }
      }
      
      //write text
      sprintf(textMenu1, "Start Game");
      sprintf(textMenu2, "See Record");
      sprintf(textMenu3, "Shop Menu");
      sprintf(textMenu4, "exit");
      cpDrawText(255, 255, (currentMenu==1?0:250), 400, 350, textMenu1, big_font2, 1);
      cpDrawText(255, 255, (currentMenu==2?0:250), 400, 350+40, textMenu2, big_font2, 1); 
      cpDrawText(255, 255, (currentMenu==3?0:250), 400, 350+80, textMenu3, big_font2, 1);
      cpDrawText(255, 255, (currentMenu==4?0:250), 400, 350+120, textMenu4, big_font2, 1);  
      cpSwapBuffers(); 
      cpDelay(10);
   } 

   return currentMenu;
}

int scorePage(){
   return 0;
}