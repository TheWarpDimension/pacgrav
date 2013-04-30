#ifndef INCLUDED_BUFFERED_FILE_READER
#define INCLUDED_BUFFERED_FILE_READER


#include <stdio.h>


class BufferedFileReader
{
public:
    FILE *m_in;
    char *m_buf;
    int  m_bufSize;
    int  m_pos;             // Pos of next byte to return
    int  m_bytesInBuf;      // Amount of data in the buffer

public:
    BufferedFileReader(FILE *in, int bufSize = 65536);
    ~BufferedFileReader();

    char *ReadChunk(int *numBytesAvailable);       // Returns a pointer to the data and populates numBytesAvailable
    bool IsEndOfFile();
};


#endif
