#include "AOI.h"
namespace kabi
{
    //logger* logger::instance = nullptr;
static AOIWorld* g_world = NULL;
AOIWorld* AOIWorld::get_global_world() {  
    return g_world;
}
void AOIWorld::init_global_world(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count)
{
    g_world = new AOIWorld(_x_begin, _x_end, _y_begin, _y_end, _x_count, _y_count);
}
AOIWorld::AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count)
    : x_begin(_x_begin), x_end(_x_end), y_begin(_y_begin), y_end(_y_end), x_count(_x_count), y_count(_y_count)
{
    x_width = (x_end - x_begin) / x_count;
    y_width = (y_end - y_begin) / y_count;
    for(int i = 0; i < x_count * y_count; ++i)
    {
        Grid tmp;
        m_grids.push_back(tmp);
    }
}
bool AOIWorld::add_player(Player::s_ptr player)
{
    /*计算所属网格号*/
    int grid_id = (player->get_x() - x_begin)/x_width + (player->get_y())/y_width * x_count;
    /*添加到该网格中*/
    m_grids[grid_id].add_player(player);
    return true;

}
bool AOIWorld::del_player(Player::s_ptr player)
{
    int grid_id = (player->get_x() - x_begin)/x_width + (player->get_y())/y_width * x_count;
    m_grids[grid_id].del_player(player);
    return true;
}
std::list<Player::s_ptr> AOIWorld::get_srd_players(Player::s_ptr player)
{
    std::list<Player::s_ptr>ret;
    /*计算所属编号*/
    int grid_id = (player->get_x() - x_begin)/x_width + (player->get_y())/y_width * x_count; 
    /*判断具体情况， 取出邻居网格的玩家*/
    int x_index = grid_id % x_count;
    int y_index = grid_id / x_count;
    if(x_index > 0 && y_index > 0)
    {
        std::list<Player::s_ptr> cur_list = m_grids[grid_id - 1 - x_count].get_players();    
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
        cur_list = m_grids[grid_id - x_count].get_players();    
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if(x_index < x_count - 1 && y_index > 0)
    {
        std::list<Player::s_ptr> cur_list = m_grids[grid_id + 1 - x_count].get_players();
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if(x_index > 0)
    {
        std::list<Player::s_ptr> cur_list = m_grids[grid_id - 1].get_players();
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if(x_index < x_count - 1)
    {
        std::list<Player::s_ptr> cur_list = m_grids[grid_id + 1].get_players();
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if(x_index > 0 && y_index < y_count - 1)
    {
        std::list<Player::s_ptr> cur_list = m_grids[grid_id - 1 + x_count].get_players();
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if(x_index < x_count - 1 && y_index < y_count - 1)
    {
        std::list<Player::s_ptr> cur_list = m_grids[grid_id + 1 + x_count].get_players();
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
        cur_list = m_grids[grid_id + x_count].get_players();    
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }

    return ret;
}
std::list<Player::s_ptr>& Grid::get_players()
{
    return m_players;
}
void Grid::add_player(Player::s_ptr player)
{
    m_players.push_back(player);
}
void Grid::del_player(Player::s_ptr player)
{
    m_players.remove(player);
}

}