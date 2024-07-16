#include "rpc_controller.h"
namespace kabi
{
void rpcController::Reset()
{
    m_error_code = 0;
    m_error_info = "";
    m_req_id = "";
    m_is_failed = false;
    m_is_cancled = false;
    m_is_finished = false;
    m_local_addr = nullptr;
    m_peer_addr = nullptr;
    m_timeout = 1000;   // ms

}

bool rpcController::Failed() const
{
    return m_is_failed;
}
std::string rpcController::ErrorText() const {
  return m_error_info;
}
void rpcController::StartCancel() {
  m_is_cancled = true;
  m_is_failed = true;
  SetFinished(true);
}

void rpcController::SetFailed(const std::string& reason) {
  m_error_info = reason;
  m_is_failed = true;
}

bool rpcController::IsCanceled() const {
  return m_is_cancled;
}

void rpcController::NotifyOnCancel(google::protobuf::Closure* callback) {

}

//子类方法
void rpcController::SetError(int32_t error_code, const std::string error_info) {
  m_error_code = error_code;
  m_error_info = error_info;
  m_is_failed = true;
}
int32_t rpcController::GetErrorCode() {
  return m_error_code;
}
std::string rpcController::GetErrorInfo() {
  return m_error_info;
}

void rpcController::SetReqId(const std::string& req_id) {
  m_req_id = req_id;
}
std::string rpcController::GetReqId() {
  return m_req_id;
}

void rpcController::SetLocalAddr(netAddr::s_ptr addr) {
  m_local_addr = addr;
}
void rpcController::SetPeerAddr(netAddr::s_ptr addr) {
  m_peer_addr = addr;
}
netAddr::s_ptr rpcController::GetLocalAddr() {
  return m_local_addr;
}
netAddr::s_ptr rpcController::GetPeerAddr() {
  return m_peer_addr;
}

void rpcController::SetTimeout(int timeout) {
  m_timeout = timeout;
}

int rpcController::GetTimeout() {
  return m_timeout;
}
bool rpcController::Finished() {
  return m_is_finished;
}
void rpcController::SetFinished(bool value) {
  m_is_finished = value;
}

}