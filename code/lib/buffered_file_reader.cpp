#include "lib/universal_include.h"

#include "lib/buffered_file_reader.h"


BufferedFileReader::BufferedFileReader(FILE *in, int bufSize /* = 655536 */)
:   m_in(in),
    m_bufSize(bufSize)
{
    m_buf = new char [bufSize];
}


BufferedFileReader::~BufferedFileReader()
{
    SafeArrayDelete(m_buf);
}


bool BufferedFileReader::IsEndOfFile()
{
    if (m_pos < m_bytesInBuf)
        return false;
    return feof(m_in);
}


char *BufferedFileReader::ReadChunk(int *numBytesAvailable)
{
    if (m_pos >= m_bytesInBuf)
    {
        m_bytesInBuf = fread(m_buf, 1, m_bufSize, m_in);
        m_pos = 0;
    }

    *numBytesAvailable = m_bytesInBuf - m_pos;
    if (*numBytesAvailable <= 0)
        return NULL;

    return m_buf + m_pos;
}