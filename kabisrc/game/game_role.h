#ifndef KABI_KABISRC_GAME_GAME_ROLE_H
#define KABI_KABISRC_GAME_GAME_ROLE_H
#include <string>
#include <memory>
#include "../net/coder/tinypb_protocol.h"
#include "AOI.h"
namespace kabi
{
class UserData
{
public:
    typedef std::shared_ptr<UserData> s_ptr;
private:
    std::string m_id;
};
class Irole
{
public:
    virtual bool init() = 0;
    virtual UserData* proc_msg(UserData::s_ptr) = 0;
    virtual void fini() = 0;
};
class GameRole : public Irole, public Player
{
public:
    GameRole();
    virtual ~GameRole();
    virtual bool init() override;
    virtual UserData* proc_msg(UserData::s_ptr) override;
    virtual void fini() override;
    virtual int get_x() override;
    virtual int get_y() override;
    virtual ~GameRole();
    tinyPBProtocol::s_ptr m_protocol {nullptr};
};
}
#endif