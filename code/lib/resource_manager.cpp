#include "lib/universal_include.h"

#include "lib/bitmap.h"
#include "lib/filesys_utils.h"
#include "lib/resource_manager.h"
#include "lib/string_utils.h"


ResourceManager *g_resourceManager;


ResourceManager::ResourceManager(char const *dataPath)
{
	ReleaseAssert(dataPath, "ResourceMananger: dataPath was NULL");

	FileDetails path(dataPath);
	ReleaseAssert(path.Exists(), "ResourceManager: data path '%s' does not exist", dataPath);

	int len = strlen(path.GetDirectoryPart());
	ReleaseAssert(len < MAX_PATH, "ResourceManager: dataPath too long");
	
	strcpy(m_dataPath, path.GetDirectoryPart());
	m_dataPathEnd = m_dataPath + len;
}

