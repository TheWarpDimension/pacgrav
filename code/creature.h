#ifndef INCLUDE_CREATURE
#define INCLUDE_CREATURE


typedef enum
{
	MoveNone,
	MoveRight,
	MoveDown,
	MoveLeft,
	MoveUp,
	MoveNumMoves
} Move;


typedef enum
{
	CreatureTypePcMan,
	CreatureTypeGhost
} CreatureType;


class Creature
{
public:
	double  m_posX;
	double  m_posY;
	double  m_posOldX;
	double  m_posOldY;
	Move    m_move;
	Move    m_nextMove;	
	double  m_speed;
	double  m_spriteFrame;
	int     m_creatureType;

	virtual void Init(double x, double y, CreatureType _npcType);
	virtual void Advance(float timeDelta);
	virtual void Render() = 0;
};


#endif
