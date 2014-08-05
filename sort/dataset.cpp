#include <stdio.h>
#include <stdlib.h>
#include "pos.h"
#include "dataset.h"
#include "func.h"

dataset::dataset() {
  size_y = size_x = 0;
}

dataset::dataset(int size_x, int size_y) {
  if(size_x < 0 || size_y < 0) exit(1);
  this->size_y = size_y;
  this->size_x = size_x;
  make_array();
  disp_data();
}

dataset::~dataset() {
  delete_array();
}

void dataset::delete_array() {
  int i, j;

  for(i = 0; i < this->size_y; i++) {
    delete [] data[i];
    delete [] distance[i];
  }
  delete [] data;
  delete [] distance;
}

// xとyに負の値を入れると[]を付けない
void dataset::disp_data(int x, int y) {
  int i, j;

  for(i = 0; i < this->size_x; i++) {
    for(j = 0; j < this->size_y; j++) {
      if(x == j && y == i) {
        printf("[%X%X]", this->data[i][j].x, this->data[i][j].y);
      }else {
        printf(" %X%X ", this->data[i][j].x, this->data[i][j].y);
      }
    }
    puts("");
  }
  puts("");
}

void dataset::disp_distance(int x, int y) {
  int i, j;

  if(!check_in_scope(size_x, size_y, x, y)) exit(1);

  for(i = 0; i < this->size_x; i++) {
    for(j = 0; j < this->size_y; j++) {
      printf("[%2d,%2d]", this->distance[i][j].x, this->distance[i][j].y);
    }
    puts("");
  }
  puts("");
}

void dataset::make_array() {
  int i, j;

  data = new pos*[size_y];
  distance = new pos*[size_y];
  for(i = 0; i < this->size_y; i++) {
    data[i] = new pos[size_x];
    distance[i] = new pos[size_x];
    for(j = 0; j < this->size_x; j++) {
      data[i][j].x = j;
      data[i][j].y = i;
      distance[i][j].x = 0;
      distance[i][j].y = 0;
    }
  }
}

void dataset::reset_data() {
  int i, j;

  if(size_y < 0 || size_x < 0) exit(1);

  for(i = 0; i < this->size_y; i++) {
    distance[i] = new pos[size_x];
    for(j = 0; j < this->size_x; j++) {
      data[i][j].x = j;
      data[i][j].y = i;
      distance[i][j].x = 0;
      distance[i][j].y = 0;
    }
  }
}

void dataset::set_distance(int x, int y) {
  int distance_x, distance_y;

  if(!check_in_scope(size_x, size_y, x, y)) exit(1);
  distance[y][x].x = x - data[y][x].x;
  distance[y][x].y = y - data[y][x].y;
}

void dataset::swap_data(int x1, int y1, int x2, int y2) {
  if(!check_in_scope(size_x, size_y, x1, y1)) exit(1);
  if(!check_in_scope(size_x, size_y, x2, y2)) exit(1);

  change_num(&data[y1][x1].x, &data[y2][x2].x);
  change_num(&data[y1][x1].y, &data[y2][x2].y);
  set_distance(x1, y1);
  set_distance(x2, y2);
}

void dataset::swap_next(int x, int y, int direction) {
  int dx = x;
  int dy = y;

  if(!check_in_scope(size_x, size_y, x, y)) exit(1);
  if(direction < 0 || direction > 3) exit(1);
  surroundings(&dx, &dy, direction);
  swap_data(dx, dy, x, y);
}
