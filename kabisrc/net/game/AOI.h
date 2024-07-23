#ifndef KABI_KABISRC_GAME_AOI_H
#define KABI_KABISRC_GAME_AOI_H
#include <list>
#include <vector>
#include <memory>
namespace kabi
{
/*拥有横纵坐标的对象*/
class Player
{
public:
    typedef std::shared_ptr<Player> s_ptr;

public:
    virtual int get_x() = 0;
    virtual int get_y() = 0;
    virtual int get_z() = 0;
    virtual int get_v() = 0;
    int x;
    int y;
    int z;
    int v;

};
class Grid
{
public:
    std::list<Player::s_ptr>& get_players();
    void add_player(Player::s_ptr player);
    void del_player(Player::s_ptr player);

private:
    std::list<Player::s_ptr>m_players;

};

class AOIWorld
{
public:
    std::vector<Grid> m_grids;
    /*通过构造函数指定矩阵的大小和分割粒度*/
    AOIWorld(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count);   
    /*获取周围玩家*/
    std::list<Player::s_ptr> get_srd_players(Player::s_ptr player);
    /*添加玩家到AOI网格*/
    bool add_player(Player::s_ptr player);
    /*删除玩家*/
    bool del_player(Player::s_ptr player);
    static AOIWorld* get_global_world();
    static void init_global_world(int _x_begin, int _x_end, int _y_begin, int _y_end, int _x_count, int _y_count);
private:
    int x_begin {0};
    int x_end = {0};
    int y_begin = (0);
    int y_end {0};
    int x_count {0};
    int y_count {0};
    int x_width {0};
    int y_width {0};
};
}
#endif