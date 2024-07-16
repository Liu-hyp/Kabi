#ifndef KABI_NET_RPC_RPC_CLOSURE_H
#define KABI_NET_RPC_RPC_CLOSURE_H

#include <google/protobuf/stubs/callback.h>
#include <functional>
#include <memory>
#include "../../include/log.h"
namespace kabi {

class RpcClosure : public google::protobuf::Closure {
public:

  void Run() override 
  {
    // 更新 runtime 的 RpcInterFace, 这里在执行 cb 的时候，都会以 RpcInterface 找到对应的接口，实现打印 app 日志等
    if(m_cb != nullptr)
    {
      m_cb();
    }
  
  
  }

private:
  std::function<void()> m_cb {nullptr};

};

}
#endif