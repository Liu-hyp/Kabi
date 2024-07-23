#ifndef KABI_KABISRC_NET_GAME_GAME_MSG_H
#define KABI_KABISRC_NET_GAME_GAME_MSG_H
#include "GameMsg.pb.h"
#include <vector>
#include <string>
#include <google/protobuf/message.h>
namespace kabi
{
enum class MessageType
{
    MSG_NONE = 0,
    MSG_SYNC_PID = 1,
    MSG_TALK = 2,
    MSG_POSITION = 3,
    MSG_BROADCAST = 200,
    MSG_SYNCPID = 201,
    MSG_SYNC_PLAYERS = 202,
    MSG_LOGIN_REQUEST = 1001,
    MSG_LOGIN_REPLY = 1002,
};

enum class User
{
    SERVER = 0,
    CLIENT = 1,
};

class GameMsg
{
public:
    // 使用MessageOption指定的ID注册
    void init_proto_registry();
    uint32_t get_message_id(google::protobuf::Message* msg);
    google::protobuf::Message* create_message(MessageID msgId);
private:
    MessageType msg_type;
    User user_type;
    std::string buffer;
};
}
#endif