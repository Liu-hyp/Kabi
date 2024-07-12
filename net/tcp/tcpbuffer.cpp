#include "tcpbuffer.h"
#include "../../include/log.h"
#include <memory>
#include <string.h>
namespace kabi
{
tcpBuffer::tcpBuffer(int size):m_size(size)
{
    m_buffer.resize(m_size);

}
tcpBuffer::~tcpBuffer()
{

}
    //返回可读字节数
int tcpBuffer::read_able()
{
    return m_write_idx - m_read_idx;
}

    //返回可写字节数
int tcpBuffer::write_able()
{
    return m_size - m_write_idx;
}
int tcpBuffer::read_index()
{
    return m_read_idx;
}
int tcpBuffer::write_index()
{
    return m_write_idx;
}
void tcpBuffer::write_buffer(const char* buf, int size)
{
    if(size > write_able())
    {
        int new_size = (int)(1.5 * (m_write_idx + size));
        resize_buffer(new_size);
    }
    memcpy(&m_buffer[m_write_idx], buf, size);
    m_write_idx += size;
}
void tcpBuffer::read_from_buffer(std::vector<char>& re, int size)
{
    if(read_able() == 0)
    {
        return;
    }
    int read_size = read_able() > size ? size : read_able();
    std::vector<char>tmp(read_size);
    memcpy(&tmp[0], &m_buffer[m_read_idx], read_size);
    re.swap(tmp);
    m_read_idx += read_size;
    tmp.clear();
    adjust_buffer();
}
void tcpBuffer::resize_buffer(int new_size)
{
    std::vector<char> tmp(new_size);
    int count = std::min(new_size, read_able());
    memcpy(&tmp[0], &m_buffer[m_read_idx], count);
    m_buffer.swap(tmp);
    m_read_idx = 0;
    m_write_idx = m_read_idx + count;
    m_size = m_buffer.size();
    tmp.clear();
    
}
void tcpBuffer::adjust_buffer()
{
    if(m_read_idx < int(m_buffer.size() / 3))
    {
        return;
    }
    int count = read_able();
    std::vector<char>buffer(m_buffer.size());
    memcpy(&buffer[0], &m_buffer[m_read_idx], count);
    m_buffer.swap(buffer);
    m_read_idx = 0;
    m_write_idx = m_read_idx + count;
    buffer.clear();
}
void tcpBuffer::move_read_index(int size)
{
    size_t j = m_read_idx + size;
    if(j >= m_buffer.size())
    {
        ERRORLOG("move_read_index error, invalid size %d, old_read_index %d, buffer size %d", size, m_read_idx, m_buffer.size());
        return;
    }
    m_read_idx = j;
    adjust_buffer();
}
void tcpBuffer::move_write_index(int size)
{
    size_t j = m_write_idx + size;
    if(j >= m_buffer.size())
    {
        ERRORLOG("move_write_index error, invalid size %d, old_write_index %d, buffer size %d", size, m_write_idx, m_buffer.size());
        return;
    }
    m_write_idx = j;
    adjust_buffer();
}
}