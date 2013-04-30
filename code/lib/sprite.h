#ifndef INCLUDED_SPRITE_H
#define INCLUDED_SPRITE_H


class Sprite
{
private:
	int m_width;
	int m_height;
	unsigned int m_textureId;

public:
	enum Mode
	{
		ModeNormal,
		ModeFlipH,
		ModeRotate90,
		ModeRotate270
	};

	Sprite();
	bool Load(char const *filename, int rotateAmount);
	void Render(int posX, int posY, Mode mode, int w=25, int h=25);
    int GetHeight() { return m_height; }
    int GetWidth() { return m_width; }
};


#endif
