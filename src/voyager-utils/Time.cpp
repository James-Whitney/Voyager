#include "include/Time.h"

using namespace std;

double currentTimeMs() {
   return KILO * clock() / (double)CLOCKS_PER_SEC;
}

LoopTimer::LoopTimer(double max_time_step) :
   max_time_step(max_time_step)
{}

void LoopTimer::init() {
   last_time = cur_time = currentTimeMs();
   initialized = true;
}

void LoopTimer::reset() {

   if (!this->initialized) {
      this->init();
   }

   last_time = cur_time;
   cur_time = currentTimeMs();

   delta_time += cur_time - last_time;

}

bool LoopTimer::tick(double *t) {

   *t = min(this->delta_time, this->max_time_step);
   this->delta_time -= *t;
   return *t > MILLI;

}
