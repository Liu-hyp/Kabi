

#include "../../message.pb.h"
#include <vector>
#include <string>
#include <google/protobuf/message.h>

typedef std::string Buffer;
// 使用MessageOption指定的ID注册
void initProtoRegistryV2();
uint32_t getMessageID(google::protobuf::Message*);
google::protobuf::Message* createMessageV2(protocol::MessageID msgId);

