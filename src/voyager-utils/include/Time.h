#pragma once
#ifndef _TIME_H
#define _TIME_H

#include <algorithm>
#include <ctime>

#include "Constants.h"

double currentTimeMs();

class LoopTimer {

public:

   LoopTimer(double max_time_step);

   void init();
   void reset();
   bool tick(double *t);

private:

   bool initialized = false;

   double max_time_step;

   double last_time;
   double cur_time;
   double delta_time;

};

#endif
