#include "game_msg.h"
#include <google/protobuf/descriptor_database.h>
#include <unordered_map>


namespace kabi
{
static std::unordered_map<MessageID, const google::protobuf::Descriptor*> registry;
// 是否以`suffix`结尾
static bool endsWith(const std::string& name, const std::string& suffix)
{
    if (name.length() < suffix.length())
    {
        return false;
    }
    int j = name.length() - 1;
    for (int i = suffix.length() - 1; i > 0; --i, --j)
    {
        if (suffix[i] != name[j])
            return false;
    }
    return true;
}
// 是否以`prefix`开头
static bool startsWith(const std::string& name, const std::string& prefix) 
{
    if (name.length() < prefix.length())
    {
        return false;
    }
    int j = 0;
    for (int i = 0; i < prefix.length(); i++, j++)
    {
        if (prefix[i] != name[j])
            return false;
    }
    return true;
}
// Req代表请求消息
// Ack代表响应消息
// Ntf通知消息
static bool hasSuffix(const std::string& name) 
{
    return endsWith(name, "Ntf") || endsWith(name, "Req") || endsWith(name, "Ack");
}
void GameMsg::init_proto_registry()
{
    const google::protobuf::DescriptorPool* pool = google::protobuf::DescriptorPool::generated_pool();
    google::protobuf::DescriptorDatabase* db = pool->internal_generated_database();
    if (db == nullptr) {
        return;
    }
    std::vector<std::string> file_names;
    db->FindAllFileNames(&file_names);
    for (const std::string& filename : file_names)
    {
        const google::protobuf::FileDescriptor* fileDescriptor = pool->FindFileByName(filename);
        if (fileDescriptor == nullptr)
        {
            continue;
        }
        int msgcount = fileDescriptor->message_type_count();
        for (int i = 0; i < msgcount; i++)
        {
            const google::protobuf::Descriptor* descriptor = fileDescriptor->message_type(i);
            if (descriptor != nullptr)
            {
                const std::string& name = descriptor->full_name();
                if (hasSuffix(name)) 
                {
                    auto opts = descriptor->options();
                    MessageID v = opts.GetExtension(MsgID);
                    registry[v] = descriptor;
                }
            }
        }
    }

}
uint32_t GameMsg::get_message_id(google::protobuf::Message* msg)
{
    return 0;
}
google::protobuf::Message* GameMsg::create_message(MessageID msgId)
{
    auto iter = registry.find(msgId);
    if (iter == registry.end())
    {
        return nullptr;
    }
    const google::protobuf::Message* protoType = google::protobuf::MessageFactory::generated_factory()->GetPrototype(iter->second);
    if (protoType != nullptr)
    {
        return protoType->New();
    }
    return nullptr;
}
}