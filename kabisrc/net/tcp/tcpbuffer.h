#ifndef KABI_KABISRC_NET_TCP_TCPBUFFER_H
#define KABI_KABISRC_NET_TCP_TCPBUFFER_H
#include <vector>
#include <memory>
namespace kabi
{
class tcpBuffer
{
public:
    typedef std::shared_ptr<tcpBuffer> s_ptr;
    tcpBuffer(int size);
    ~tcpBuffer();
    //返回可读字节数
    int read_able();
    //返回可写字节数
    int write_able();
    int read_index();
    int write_index();
    void write_buffer(const char* buf, int size);
    void read_from_buffer(std::vector<char>& re, int size);
    void resize_buffer(int new_size);
    void adjust_buffer();
    void move_read_index(int size);
    void move_write_index(int size);
private:
    int m_read_idx {0};
    int m_write_idx {0};
    int m_size {0};
public:
    std::vector<char>m_buffer;

};
}
#endif //KABI_NET_TCP_TCPBUFFER_H