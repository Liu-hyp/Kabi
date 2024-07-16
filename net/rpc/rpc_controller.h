#ifndef KABI_NET_RPC_RPC_CONTROLLER_H
#define KABI_NET_RPC_RPC_CONTROLLER_H

#include <google/protobuf/service.h>
#include <google/protobuf/stubs/callback.h>
#include <string>

#include "../../net/tcp/net_addr.h"
#include "../../include/log.h"

namespace kabi 
{
//rpcController相当于rpc的配置文件
class rpcController : public google::protobuf::RpcController {

 public:
  rpcController() { INFOLOG("RpcController"); } 
  ~rpcController() { INFOLOG("~RpcController"); } 
//继承父类的纯虚函数
  void Reset(); //作为rpc客户端的调用方法，复用rpcController

  bool Failed() const;//获取当前rpc是否成功

  std::string ErrorText() const;

  void StartCancel();

  void SetFailed(const std::string& reason);

  bool IsCanceled() const;

  void NotifyOnCancel(google::protobuf::Closure* callback);

public:

  void SetError(int32_t error_code, const std::string error_info);

  int32_t GetErrorCode();

  std::string GetErrorInfo();

  void SetReqId(const std::string& req_id);

  std::string GetReqId();

  void SetLocalAddr(netAddr::s_ptr addr);

  void SetPeerAddr(netAddr::s_ptr addr);

  netAddr::s_ptr GetLocalAddr();

  netAddr::s_ptr GetPeerAddr();

  void SetTimeout(int timeout);

  int GetTimeout();

  bool Finished();

  void SetFinished(bool value);
 
 private:
  int32_t m_error_code {0};
  std::string m_error_info;
  std::string m_req_id;

  bool m_is_failed {false};
  bool m_is_cancled {false};
  bool m_is_finished {false};

  netAddr::s_ptr m_local_addr; //本地地址
  netAddr::s_ptr m_peer_addr; //对端地址

  int m_timeout {1000};   // ms 超时时间

};

}


#endif