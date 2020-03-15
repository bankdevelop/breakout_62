#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "cp_functions.h"

int startPage(char WindowTitle[], int WindowWidth, int WindowHeight){
   if (!cpInit(WindowTitle, WindowWidth, WindowHeight)) {
      fprintf(stderr, "Window initialization failed!\n");
      exit(1);
   }

   return 0;
}