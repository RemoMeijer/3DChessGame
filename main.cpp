#include <thread>

#include "Graphics.h"

void graphicsLoop();

Graphics graphics;


int main()
{
   graphics = Graphics();
   std::thread graphicsThread = std::thread(&graphicsLoop);

   bool playing = true;

   while (playing) {
      // todo implemenmt
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
   }

   return 0;
}

void graphicsLoop() {
   graphics.mainLoop();
}