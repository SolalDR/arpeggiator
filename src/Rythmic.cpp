#include "Rythmic.h"
using namespace std;

Rythmic::Rythmic() {
  this->length = 16;
  this->mode = MODE_LIVE;
  this->pointer = 0;
}


int Rythmic::advance() {
  if(this->mode == MODE_LIVE) {
    this->pointer = 0;
  } else {
    this->pointer = (this->pointer + 1) % this->length;
  }
}

RythmicTick Rythmic::getTick() {
  return this->ticks[this->pointer];
}

RythmicTick Rythmic::computeTick() {
  RythmicTick tick = this->getTick();

  // Compute duration
  this->tmp.duration = this->tickBaseLength*tick.duration; // Base duration
  this->tmp.duration -= this->tmp.duration*this->tickRandomLength;

  this->tmp.velocity = 100*this->tickBaseVelocity;
  this->tmp.velocity -= this->tmp.velocity*this->tickRandomVelocity;

  return this->tmp;
}

int Rythmic::setLength(int length) {
  this->length = length > LENGTH_MAX 
    ? LENGTH_MAX
    : length;

  return this->length;
}

int Rythmic::getLength() {
  return this->length;
}
