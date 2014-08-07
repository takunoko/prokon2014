#include <stdio.h>
#include <stdlib.h>
#include "pos.h"
#include "dataset.h"
#include "util.h"

dataset::dataset() {
  height = width = selected_num = changed_num = move_flag = 0;
}

dataset::dataset(int w, int h) {
  if(w < 0 || h < 0) exit(1);
  this->width = w;
  this->height = h;
  this->selected_num = this->changed_num = this->move_flag = 0;
  make_array();
}

dataset::~dataset() {
  delete_array();
}

void dataset::delete_array() {
  int i, j;

  for(i = 0; i < this->height; i++) {
    delete [] data[i];
    delete [] distance[i];
  }
  delete [] data;
  delete [] distance;
}

void dataset::disp_cost() {
  printf("(selected=%d, changed=%d)\n", selected_num, changed_num);
}

// xとyに負の値を入れると[]を付けない
void dataset::disp_data(int x, int y) {
  int i, j;

  for(i = 0; i < this->width; i++) {
    for(j = 0; j < this->height; j++) {
      if(check_pos_equal(j, i, x, y) || check_pos_equal(j, i, selected.x, selected.y)) {
        printf("[%X%X]", this->data[i][j].x, this->data[i][j].y);
      } else {
        printf(" %X%X ", this->data[i][j].x, this->data[i][j].y);
      }
    }
    puts("");
  }
  puts("");
}

void dataset::disp_distance() {
  int i, j;

  for(i = 0; i < this->width; i++) {
    for(j = 0; j < this->height; j++) {
      printf("[%2d,%2d]", this->distance[i][j].x, this->distance[i][j].y);
    }
    puts("");
  }
  puts("");
}

int dataset::find_data(int data_x, int data_y, int *x, int *y) {
  int i, j;

  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      if(check_pos_equal(this->data[i][j].x, this->data[i][j].y, data_x, data_y)) {
        *x = j;
        *y = i;
        return 0;
      }
    }
  }
  return 1;
}

int dataset::get_width() {
  return this->width;
}

int dataset::get_height() {
  return this->height;
}

void dataset::make_array() {
  int i, j;

  data = new pos*[height];
  distance = new pos*[height];
  for(i = 0; i < this->height; i++) {
    data[i] = new pos[width];
    distance[i] = new pos[width];
    for(j = 0; j < this->width; j++) {
      data[i][j].x = j;
      data[i][j].y = i;
      distance[i][j].x = 0;
      distance[i][j].y = 0;
    }
  }
}

void dataset::randomize_data() {
  int i, j, x, y, w, h;

  reset_data();

  for(i = 0; i < height; i++) {
    for(j = 0; j < width; j++) {
      x = rand() % width;
      y = rand() % height;
      swap_data(j, i, x, y);
    }
  }
}

void dataset::reset_data() {
  int i, j;

  for(i = 0; i < this->height; i++) {
    for(j = 0; j < this->width; j++) {
      this->data[i][j].x = j;
      this->data[i][j].y = i;
      this->distance[i][j].x = 0;
      this->distance[i][j].y = 0;
    }
  }
  selected_num = changed_num = move_flag = 0;
}

void dataset::select_data(int x, int y) {
  selected.x = x;
  selected.y = y;
  move_flag = 0;
}

void dataset::set_distance(int x, int y) {
  int distance_x, distance_y;

  if(!check_in_scope(width, height, x, y)) exit(1);
  distance[y][x].x = x - data[y][x].x;
  distance[y][x].y = y - data[y][x].y;
}

void dataset::swap_data(int x1, int y1, int x2, int y2) {
  if(!check_in_scope(width, height, x1, y1)) exit(1);
  if(!check_in_scope(width, height, x2, y2)) exit(1);

  swap_pos(&data[y1][x1], &data[y2][x2]);
  /*
  change_num(&data[y1][x1].x, &data[y2][x2].x);
  change_num(&data[y1][x1].y, &data[y2][x2].y);
  */
  set_distance(x1, y1);
  set_distance(x2, y2);
}

void dataset::swap_next(int x, int y, int direction) {
  int dx = x;
  int dy = y;

  if(!check_in_scope(width, height, x, y)) exit(1);
  if(direction < 0 || direction > 3) exit(1);
  surroundings(&dx, &dy, direction);
  swap_data(dx, dy, x, y);
}

void dataset::swap_selected(int direction) {
  swap_next(selected.x, selected.y, direction);
  surroundings(&selected.x, &selected.y, direction);
  changed_num++;
  if(move_flag == 0) {
    selected_num++;
    move_flag = 1;
  }
}
