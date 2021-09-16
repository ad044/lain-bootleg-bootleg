#pragma once
typedef struct {
  char ampm[3];
  char hour[3];
  char minute[3];
} GameTime;

GameTime get_current_time();
void update_time(GameTime *current_time);
  

