

#include "../../message.pb.h"
#include <vector>
#include <string>
#include <google/protobuf/message.h>
//     // 消息ID枚举
// enum MessageID {
//     MSG_NONE = 0,
//     MSG_DISCONNECT_NOTIFY = 1000,
//     MSG_LOGIN_REQUEST = 1001,
//     MSG_LOGIN_REPLY = 1002,
// };
typedef std::string Buffer;
// 使用MessageOption指定的ID注册
void initProtoRegistryV2();
uint32_t getMessageID(google::protobuf::Message*);
google::protobuf::Message* createMessageV2(protocol::MessageID msgId);

