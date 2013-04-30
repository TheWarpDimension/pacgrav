#ifndef INCLUDED_GHOST_H
#define INCLUDED_GHOST_H


#include "lib/sprite.h"
#include "creature.h"
// test

typedef enum
{
	GhostModeScatter,
	GhostModeNormal,
	GhostModeRunAway,
	GhostModeEyesOnly
} GhostMode;


typedef enum
{
	GhostCharacterRed,
	GhostCharacterBlue,
	GhostCharacterOrange,
	GhostCharacterPink
} GhostCharacter;


class Ghost: public Creature
{
private:
	GhostMode m_mode;
	GhostCharacter m_character;

	bool IsPositionAllowed(int x, int y);
	bool IsMoveAllowed(Move m);
	Move GetBestMoveTowardsPcMan();
	void AdvanceScatterMovement();
	void AdvanceNormalMovement(Move oldMove);
	void AdvanceEyesOnlyMovement();

public:
	Sprite m_eyesOnlySprite;
	Sprite m_normalEyesSprite;
	Sprite m_runAwayEyesSprite;
	Sprite m_normalSprites[4];
	Sprite m_runAwaySprites[4];

	void Init(double x, double y, GhostCharacter character);
	void Advance(float timeDelta);
	void Render();
	void EndPowerPillMode();
	void EnterNormalMode();
	void EnterScatterMode();
	void SetModeRunAway();
};


#endif
