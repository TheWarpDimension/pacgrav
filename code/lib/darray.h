#ifndef INCLUDED_DARRAY_H
#define INCLUDED_DARRAY_H

// A dynamic array template class
// 
// Automatically grows as more data is added
// An entry's index never changes

template <class T>
class DArray
{
protected:
    unsigned int	m_arraySize;
	unsigned int	m_nextIndex;				// One greater than the head item
    T				*m_array;
    
	inline void Grow			();

public:
    DArray();
    ~DArray();

	inline void SetSize(unsigned int newSize);

	inline T GetData	(unsigned int index) const;
	inline T *GetPointer(unsigned int index) const;

    inline unsigned int Push	(const T &newdata);		// Returns index used
	inline T Pop				();						// Returns head item and removes it
	inline T Top				();						// Returns head item

    inline unsigned int Size() const;					// Returns the number of used entries
    
    void Empty			();						// Resets the array to empty    
	void EmptyAndDelete ();						// Same as Empty() but deletes the elements that are pointed to as well
    
    inline T operator [] (unsigned int index);
	inline const T operator [] (unsigned int index) const;
};


#include "darray.cpp"

#endif
