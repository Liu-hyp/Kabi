#include "game_role.h"
#include "GameMsg.pb.h"
#include "../../include/log.h"
#include <string>
namespace kabi
{
static AOIWorld world(0, 400, 0, 400, 20, 20);
GameRole::GameRole(int fd, std::string username) : m_fd(fd), m_username(username)
{

}

GameRole::~GameRole()
{
    printf("~GameRole()");
}
bool GameRole::init()
{
    /*添加到自己的游戏世界*/
    bool bRet = false;
    this->x = 9;
    this->y = 9;
    bRet = world.add_player(this);
    /*向自己发送id和名称*/
    //create_id_name_login();
    return true;
}
void GameRole::fini()
{
    world.del_player(this);
}
int GameRole::get_x()
{
    return (int)x;
}
int GameRole::get_y()
{
    return (int)y;
}
void GameRole::create_id_name_login()
{
    // std::shared_ptr<pb::SyncPid> login_msg = std::make_shared<pb::SyncPid>();
    // login_msg->set_pid(m_fd);
    // login_msg->set_username(m_username);
    // std::shared_ptr<tinyPBProtocol> login_request = std::make_shared<tinyPBProtocol>();
    // login_request->m_msg_id = "123456";
    // if(!login_msg->SerializeToString(&(login_request->m_pb_data)))
    // {
    //     //将整个request对象转化为pb结构体
    //     ERRORLOG("serialize error");
    //     return;
    // }
    // login_request->m_method_name = "Login.makeLogin";
    // m_channel->getTcpClient()->write_msg(login_request, [login_msg](kabi::abstractProtocol::s_ptr msg_ptr){
    //     DEBUGLOG("send message success, login_msg[%s]", login_msg->ShortDebugString().c_str());
    // });
    // m_channel->getTcpClient()->read_msg("123456", [](kabi::abstractProtocol::s_ptr msg_ptr){
    //         std::shared_ptr<kabi::tinyPBProtocol> message = std::dynamic_pointer_cast<kabi::tinyPBProtocol>(msg_ptr);
    //         DEBUGLOG("msg_id %s, get response %s", message->m_msg_id.c_str(), message->m_pb_data.c_str());
    //         pb::SyncPidResponse response;
    //         if(!response.ParseFromString(message->m_pb_data))
    //         {
    //             ERRORLOG("deserialize error");
    //             return;
    //         }
    //         DEBUGLOG("get response success, response[%s]", response.ShortDebugString().c_str());
    //     });
} UserData* GameRole::proc_msg(UserData::s_ptr)
{
    return nullptr;
}
}