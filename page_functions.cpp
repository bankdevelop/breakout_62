#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <pthread.h>
#include "cp_functions.h"

Texture background_texture_menu;
Event eventPage;
Sound end_sound2;
Font big_font2;
int runningMenuPage = 1;
int currentMenu = 2;
/* 2 = Start game
   3 = See Score
   4 = Shop
   5 = exit

   speical number 1 is menu page
*/

void setQuit(){
   currentMenu = 0;
   runningMenuPage = 0;
}

int menuPage(int WindowWidth, int WindowHeight){

   end_sound2 = cpLoadSound("theEnd.wav");
   big_font2 = cpLoadFont("THSarabun.ttf", 60);
   background_texture_menu = cpLoadTexture("background.png");
   char textMenu1[80], textMenu2[80], textMenu3[80], textMenu4[80];

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
               if( ++currentMenu > 5 ) currentMenu = 2;
            } else if (eventPage.key.keysym.sym == K_UP) {
               if( --currentMenu < 2 ) currentMenu = 5;
            } else if (eventPage.key.keysym.sym == K_ESCAPE) {
               setQuit();
            } 
         } else {
            if (eventPage.type == QUIT) {
               setQuit();
            }
         }
      }
      
      //write text
      sprintf(textMenu1, "Start Game");
      sprintf(textMenu2, "See Record");
      sprintf(textMenu3, "Shop Menu");
      sprintf(textMenu4, "exit");
      cpDrawText(255, 255, (currentMenu==2?0:250), 400, 350, textMenu1, big_font2, 1);
      cpDrawText(255, 255, (currentMenu==3?0:250), 400, 350+40, textMenu2, big_font2, 1); 
      cpDrawText(255, 255, (currentMenu==4?0:250), 400, 350+80, textMenu3, big_font2, 1);
      cpDrawText(255, 255, (currentMenu==5?0:250), 400, 350+120, textMenu4, big_font2, 1);  
      cpSwapBuffers(); 
      cpDelay(10);
   } 

   return currentMenu;
}

int scorePage(){
   return 0;
}