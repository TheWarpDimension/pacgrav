#ifndef INCLUDE_PCMAN_H
#define INCLUDE_PCMAN_H


#include "creature.h"
#include "lib/sprite.h"


class PcMan: public Creature
{
public:
	int     m_numLives;
	int     m_lastMove;
	Sprite  m_sprite[4];

	virtual void Init(double x, double y);
	virtual void Advance(float timeDelta);
	virtual void Render();
};


#endif
