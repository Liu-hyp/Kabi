#include "msg_api.h"

#include <google/protobuf/descriptor_database.h>
#include <unordered_map>

using namespace google::protobuf;
using namespace protocol;

static std::unordered_map<protocol::MessageID, const Descriptor*> registry2;


const std::string package_name = "protocol";
// 是否以`suffix`结尾
static bool endsWith(const std::string& name, const std::string& suffix)
{
    if (name.length() < suffix.length())
    {
        return false;
    }
    int j = name.length() - 1;
    for (int i = suffix.length() - 1; i > 0; i--, j--)
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

static uint32_t fnvHash(const std::string& name)
{
    uint32_t hash = (2166136261);
    for (int i = 0; i < name.length(); i++)
    {
        char ch = name[i];
        hash ^= uint32(ch);
        hash *= 16777619;
    }
    return hash;
}

// 根据MessageOption指定的ID注册
void initProtoRegistryV2()
{
    const DescriptorPool* pool = DescriptorPool::generated_pool();
    DescriptorDatabase* db = pool->internal_generated_database();
    if (db == nullptr) {
        return;
    }
    std::vector<std::string> file_names;
    db->FindAllFileNames(&file_names);
    for (const std::string& filename : file_names)
    {
        const FileDescriptor* fileDescriptor = pool->FindFileByName(filename);
        if (fileDescriptor == nullptr)
        {
            continue;
        }
        int msgcount = fileDescriptor->message_type_count();
        for (int i = 0; i < msgcount; i++)
        {
            const Descriptor* descriptor = fileDescriptor->message_type(i);
            if (descriptor != nullptr)
            {
                const std::string& name = descriptor->full_name();
                if (startsWith(name, package_name)) {
                    if (hasSuffix(name)) {
                        auto opts = descriptor->options();
                        protocol::MessageID v = opts.GetExtension(protocol::MsgID);
                        registry2[v] = descriptor;
                    }
                }
            }
        }
    }
}


google::protobuf::Message* createMessageV2(protocol::MessageID msgId)
{
    auto iter = registry2.find(msgId);
    if (iter == registry2.end())
    {
        return nullptr;
    }
    const Message* protoType = MessageFactory::generated_factory()->GetPrototype(iter->second);
    if (protoType != nullptr)
    {
        return protoType->New();
    }
    return nullptr;
}