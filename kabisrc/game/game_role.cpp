#include "game_role.h"
namespace kabi
{
static AOIWorld world(0, 400, 0, 400, 20, 20);
GameRole::GameRole()
{

}

GameRole::~GameRole()
{

}
bool GameRole::init()
{
    /*添加到自己的游戏世界*/
    bool bRet = false;
    bRet = world.add_player(this);
    /*向自己发送id和名称*/
    
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
}