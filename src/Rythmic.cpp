#include "Rythmic.h"
using namespace std;

double randd() {
  return (double)rand() / ((double)RAND_MAX + 1);
}

RythmicTick * liveTick = new RythmicTick();

Rythmic::Rythmic() {
  this->pointer = 0;
  this->length = 16;
  this->mode = MODE_LIVE;

  this->tickBaseLength = 1.0;
  this->tickRandomLength = 0.1;
  this->tickBaseVelocity = 0.5;
  this->tickRandomVelocity = 0.1;

  this->tmp = new RythmicTick();
  for(int i=0; i<LENGTH_MAX; i++) {
    this->ticks[i] = new RythmicTick();
  }
}

int Rythmic::advance() {
  if(this->mode == MODE_LIVE) {
    this->pointer = 0;
  } else {
    this->pointer = (this->pointer + 1) % this->length;
  }

  return this->pointer;
}

RythmicTick * Rythmic::getTick() {
  return this->mode == MODE_LIVE 
    ? liveTick
    : this->ticks[this->pointer];
}

RythmicTick * Rythmic::computeTick() {
  RythmicTick * tick = this->getTick();

  // Compute duration
  this->tmp->duration = this->tickBaseLength*tick->duration; // Base duration
  this->tmp->duration -= this->tmp->duration*this->tickRandomLength*randd();

  this->tmp->velocity = 128*this->tickBaseVelocity;
  this->tmp->velocity -= this->tmp->velocity*this->tickRandomVelocity*randd();

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
