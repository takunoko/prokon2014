#include "Pos.h"
// ポジション

Pos::Pos() {
  x = y = -1;
}

Pos::Pos(int x, int y) {
  this->x = x;
  this->y = y;
}

void Pos::setZero() {
  this->x = this->y = 0;
}
