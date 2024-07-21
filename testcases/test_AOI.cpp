#include "../kabisrc/game/AOI.h"
#include <list>
#include <string>

class myPlayer : public kabi::Player
{
public:
    myPlayer(std::string _name, int _x, int _y): name(_name), x(_x), y(_y){}
    virtual int get_x() override
    {
        return x;
    }
    virtual int get_y() override
    {
        return y;
    }
    std::string& get_name()
    {
        return name;
    }
private:
    int x {0};
    int y {0};
    std::string name;
};
int main()
{
    kabi::AOIWorld world(20, 200, 50, 230, 6, 6);
    myPlayer p1("卡比", 60, 107);
    myPlayer p2("芒果", 91, 118);
    myPlayer p3("小白", 147, 133);
    world.add_player(&p1);
    world.add_player(&p2);
    world.add_player(&p3);
    auto srd_list = world.get_srd_players(&p1);
    for(auto single : srd_list)
    {
        dynamic_cast<myPlayer*>(single);
        printf("在player[%s]的附近有player[%s]", p1.get_name().c_str(), dynamic_cast<myPlayer*>(single)->get_name().c_str());
    }
    return 0;
}