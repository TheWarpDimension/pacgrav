#include "lib/string_utils.h"


//*****************************************************************************
// Class KeyTreeItem
//*****************************************************************************

template <class T>
KeyTreeItem<T>::KeyTreeItem(char const *key, T &newData)
:   m_data(newData),
	m_balanceFactor(0),
	m_parent(NULL),
	m_left(NULL),
    m_right(NULL)
{
	m_key = StringDuplicate(key);
}


template <class T>
KeyTreeItem<T>::~KeyTreeItem()
{
	SafeArrayDelete(m_key);
}



//*****************************************************************************
// Class KeyTree
//*****************************************************************************

template <class T>
KeyTree<T>::KeyTree()
:	m_root(NULL)
{
}


template <class T>
KeyTree<T>::~KeyTree()
{
	while (m_root) RemoveItem(m_root->m_key);
}


template <class T>
KeyTreeItem<T> **KeyTree<T>::GetPip(char const *key)
{
	KeyTreeItem<T> **pip = &m_root;
	
	while (*pip)
	{
		int cmpResult = stricmp(key, (*pip)->m_key);
		if (cmpResult > 0)
		{
			pip = &(*pip)->m_right;
		}
		else if (cmpResult < 0)
		{
			pip = &(*pip)->m_left;
		}	
		else
		{
			return pip;
		}	
	}

	return NULL;
}


template <class T>
void KeyTree<T>::PutData(char const *key, T &newData)
{
	KeyTreeItem<T> *newItem = new KeyTreeItem<T> (key, newData);
    
	if (m_root)
	{
		KeyTreeItem<T> *node = m_root;

		while (node)
		{
			int cmpResult = stricmp(key, node->m_key);
			if (cmpResult > 0)
			{
				if (node->m_right)
				{
					node = node->m_right;
				}
				else
				{
					newItem->m_parent = node;
					node->m_right = newItem;
					return;
				}	
			}
			else if (cmpResult < 0)
			{
				if (node->m_left)
				{
					node = node->m_left;
				}
				else
				{
					newItem->m_parent = node;
					node->m_left = newItem;
					return;
				}
			}	
			else
			{
				DebugAssert(0);
				return;
			}	
		}
	}
	else
	{
		m_root = newItem;
	}
}


template <class T>
T KeyTree<T>::GetData(char const *key)
{
    KeyTreeItem<T> **pip = GetPip(key);
    if (pip)
	{
	    return (*pip)->m_data;
	}

	return (T) 0;
}


template <class T>
bool KeyTree<T>::RemoveItem(char const *key)
{
	// Get a pointer to the parent's pointer to our target node
	KeyTreeItem<T> **pip = GetPip(key);
	
	if (pip)
	{
		// Get a pointer to the actual target node
		KeyTreeItem<T> *node = *pip;

		if (!node->m_left && !node->m_right)		// Is item a leaf node?
		{
			*pip = NULL;
		}
		else if (!node->m_left)						// Item has no left child
		{
			*pip = node->m_right;
			node->m_right->m_parent = node->m_parent;
		}
		else if (!node->m_right)					// Item has no right child
		{
			*pip = node->m_left;
			node->m_left->m_parent = node->m_parent;
		}
		else										// Item has two children
		{
			// Replace ourself with left
			*pip = node->m_left;
			node->m_left->m_parent = node->m_parent;

			// Re-add our right
			KeyTreeItem<T> *rightMostNode = node->m_left;
			while (rightMostNode->m_right)
			{
				rightMostNode = rightMostNode->m_right;
			}
			rightMostNode->m_right = node->m_right;
			node->m_right->m_parent = rightMostNode;
		}

		// Delete the target node
		delete node;

		return true;
	}

	return false;
}
