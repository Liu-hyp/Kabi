#ifndef KABI_KABISRC_NET_RPC_RPC_DISPATCHER_H
#define KABI_KABISRC_NET_RPC_RPC_DISPATCHER_H
#include "../coder/abstract_protocol.h"
#include "../coder/tinypb_protocol.h"
#include <map>
#include <memory>
#include <string.h>
#include <google/protobuf/service.h>
//#include "../tcp/tcp_connection.h"
namespace kabi
{
class tcpConnection;
class rpcDispatcher
{
public:
    static rpcDispatcher* get_rpc_dispatcher();
    typedef std::shared_ptr<google::protobuf::Service> service_s_ptr;
    void dispatch(abstractProtocol::s_ptr request, abstractProtocol::s_ptr response, tcpConnection* connection);
    void register_service(service_s_ptr service);
    void set_tinyPB_error(std::shared_ptr<tinyPBProtocol> msg, int32_t err_code, const std::string err_info);
private:
    bool parse_service_full_name(const std::string& full_name, std::string& service_name, std::string& method_name);
private:
    std::map<std::string, std::shared_ptr<google::protobuf::Service>>m_service_map;
};
}
#endif //KABI_NET_RPC_RPC_DISPATCHER_H