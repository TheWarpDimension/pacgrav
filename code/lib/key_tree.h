#ifndef INCLUDED_KEY_TREE_H
#define INCLUDED_KEY_TREE_H


template <class T>
class KeyTreeItem
{
public:
	char		*m_key;
	T			m_data;
	int			m_balanceFactor;
	KeyTreeItem *m_parent;
	KeyTreeItem *m_left;
	KeyTreeItem *m_right;

	KeyTreeItem(char const *key, T &newData);
	~KeyTreeItem();
};


template <class T>
class KeyTree
{
protected:
	KeyTreeItem<T> *m_root;

protected:
	// Get the pointer in parent; a pointer to the pointer to the specified item. ie the
	// address of m_left or m_right in the item's parent, or m_root
	KeyTreeItem<T> **GetPip(char const *key);	

public:
	KeyTree();
	~KeyTree();

	inline void PutData			(char const *key, T &newData );
    inline T	GetData			(char const *key);				// Returns NULL on failure
	inline bool RemoveItem		(char const *key);				// Returns true on success
};


#include "key_tree.cpp"


#endif
