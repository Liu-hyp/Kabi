#ifndef KABI_KABISRC_INCLUDE_RUNTIME_H
#define KABI_KABISRC_INCLUDE_RUNTIME_H
#include <string>
namespace kabi
{
//class rpcInterface;

class runTime {
public:
    //rpcInterface* get_rpc_interface();

public:
    static runTime* GetRunTime();


public:
    std::string m_msgid;
    std::string m_method_name;
    //rpcInterface* m_rpc_interface {NULL};
};

}
#endif