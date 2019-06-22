#pragma once

#include "Emitter.h"

class Enemy : Emitter {
public:
	Enemy() : Emitter(SpriteSystem sys);
	Emitter emitter;


};