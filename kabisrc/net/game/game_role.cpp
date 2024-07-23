#include "game_role.h"
#include "GameMsg.pb.h"
#include "../../include/log.h"
#include <string>
namespace kabi
{
GameRole::GameRole(int fd, std::string username, int _x, int _y, int _z, int _v) 
: m_id(fd), m_username(username)
{
    this->x = _x;
    this->y = _y;
    this->z = _z;
    this->v = _v;
}

GameRole::~GameRole()
{
    printf("~GameRole()");
}
bool GameRole::init()
{
    /*添加到自己的游戏世界*/
    bool bRet = false;
    bRet = AOIWorld::get_global_world()->add_player(std::shared_ptr<GameRole>(this));
    /*调用init时代表已经建立连接*/
    /*向自己发送周围玩家的位置*/
    if(bRet == true)
    {

    }
    
    return true;
}
void GameRole::fini()
{
    AOIWorld::get_global_world()->del_player(std::shared_ptr<GameRole>(this));
}
int GameRole::get_x()
{
    return (int)x;
}
int GameRole::get_y()
{
    return (int)y;
}
int GameRole::get_z()
{
    return (int)z;
}
int GameRole::get_v()
{
    return (int)v;
}
void GameRole::create_id_name_login(std::shared_ptr<SyncPid> login_msg)
{
    login_msg->set_pid(m_id);
    login_msg->set_username(m_username);    
}
void GameRole::create_srd_players(std::shared_ptr<SyncPlayers> sync_players_msg)
{
    std::shared_ptr<GameRole> role = shared_from_this();
    std::list<Player::s_ptr> srd_list = AOIWorld::get_global_world()->get_srd_players(std::dynamic_pointer_cast<Player>(role));
    for(auto single : srd_list)
    {
        auto per_player_msg = sync_players_msg->add_ps();
        auto per_role = std::dynamic_pointer_cast<GameRole>(single);
        per_player_msg->set_pid(per_role->m_id);
        auto per_position = per_player_msg->mutable_p();
        per_position->set_x(per_role->x);
        per_position->set_y(per_role->y);
        per_position->set_z(per_role->z);
        per_position->set_v(per_role->v);
    }
}
UserData* GameRole::proc_msg(UserData::s_ptr)
{
    return nullptr;
}
}