#ifndef INCLUDED_RESOURCE_MANAGER_H
#define INCLUDED_RESOURCE_MANAGER_H


#include "lib/key_tree.h"


class Sprite;


class ResourceManager
{
private:
	char m_dataPath[MAX_PATH + 1];		// Stores the path of the data directory. Also is used as a temp storage of the full resource path
	char *m_dataPathEnd;
	KeyTree <Sprite*> m_sprites;

public:
	ResourceManager(char const *dataPath);
	Sprite *GetSprite(char const *filename);
};


extern ResourceManager *g_resourceManager;


#endif
