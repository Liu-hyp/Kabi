#include "../kabisrc/game/msg_api.h"
#include <string>

void test_msg_option()
{
    initProtoRegistryV2();

    protocol::LoginReq req;
    req.set_user("user001");
    req.set_token("pnyuza0h2cdkvxvh54v3dn");
    req.set_unix_time(1615004452);
    req.set_language("zh-CN");
    req.set_client_os("Windows 10");
    req.set_app_version("1.0.1");
    std::cout << req.DebugString() << std::endl;

    auto bytes = req.SerializeAsString();
    auto msg = (protocol::LoginReq*)createMessageV2(protocol::MSG_LOGIN_REQUEST);
    if (msg != nullptr)
    {
        msg->ParseFromArray(bytes.data(), bytes.size());
        std::cout << req.DebugString() << std::endl;
    }
}

int main(int argc, char* argv[])
{
    test_msg_option();

    return 0;
}