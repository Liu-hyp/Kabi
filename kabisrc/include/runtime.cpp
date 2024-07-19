#include "runtime.h"
namespace kabi
{
thread_local runTime* t_run_time = NULL; 

runTime* runTime::GetRunTime() {
  if (t_run_time) {
    return t_run_time;
  }
  t_run_time = new runTime();
  return t_run_time;
}


// rpcInterface* runTime::get_rpc_interface() {
//   return m_rpc_interface;
// }
}