/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/
#include <stdio.h>

#include "RemotingCommand.h"
#include "MQProtos.h"
#include "MQTypes.h"

namespace lxmq{



RemotingCommand& RemotingCommand::operator=(const RemotingCommand& command) {
  if (this != &command) {
    Assign(command);
  }
  return *this;
}

RemotingCommand::~RemotingCommand() {

}

void RemotingCommand::Assign(const RemotingCommand& command) {
  m_code = command.m_code;
  m_language = command.m_language;
  m_version = command.m_version;
  m_opaque = command.m_opaque;
  m_flag = command.m_flag;
  m_remark = command.m_remark;
  m_msgBody = command.m_msgBody;

  for (auto& it : command.m_extFields) {
    m_extFields[it.first] = it.second;
  }

  m_head = command.m_head;
  m_body = command.m_body;
  m_parsedJson = command.m_parsedJson;
  // m_pExtHeader = command.m_pExtHeader; //ignore this filed at this moment, if need please add it
}

void RemotingCommand::Encode() {
  Json::Value root;
  root["code"] = m_code;
  root["language"] = "CPP";
  root["version"] = m_version;
  root["opaque"] = m_opaque;
  root["flag"] = m_flag;
  root["remark"] = m_remark;

  if (m_pExtHeader) {
    Json::Value extJson;
    m_pExtHeader->Encode(extJson);

    root["extFields"] = extJson;
  } else {  // for heartbeat
   
  }

  Json::FastWriter fastwrite;
  string data = fastwrite.write(root);

  MQUINT32 headLen = data.size();
  MQUINT32 totalLen = 4 + headLen + m_body.GetSize();

  MQUINT32 messageHeader[2];
  messageHeader[0] = htonl(totalLen);
  messageHeader[1] = htonl(headLen);

  /*m_head.setSize(4 + 4 + headLen);
  m_head.copyFrom(messageHeader, 0, sizeof(messageHeader));
  m_head.copyFrom(data.c_str(), sizeof(messageHeader), headLen);*/
}

const MQBuffer* RemotingCommand::GetHead() const {
  return &m_head;
}

const MQBuffer* RemotingCommand::GetBody() const {
  return &m_body;
}

void RemotingCommand::SetBody(const char* pData, int len) {
  /*m_body.reset();
  m_body.setSize(len);
  m_body.copyFrom(pData, 0, len);*/
}

RemotingCommand* RemotingCommand::Decode(const MQBuffer& buf) {
  //<!decode 1 bytes,4+head+body
  MQUINT32 messageHeader = 0;
  messageHeader = *(MQUINT32*)buf.GetData();
  int totalLen = buf.GetSize();
  int headLen = ntohl(messageHeader);
  int bodyLen = totalLen - 4 - headLen;

  //<!decode header;
  const char* const pData = static_cast<const char*>(buf.GetData());
  Json::Reader reader;
  Json::Value object;
  const char* begin = pData + 4;
  const char* end = pData + 4 + headLen;

  if (!reader.parse(begin, end, object)) {
    //THROW_MQEXCEPTION(MQClientException, "conn't parse json", -1);
    return NULL;
  }

  int code = object["code"].asInt();

  string language = object["language"].asString();
  int version = object["version"].asInt();
  int opaque = object["opaque"].asInt();
  int flag = object["flag"].asInt();
  Json::Value v = object["remark"];
  string remark = "";
  if (!v.isNull()) {
    remark = object["remark"].asString();
  }
  printf(
      "code:%d, remark:%s, version:%d, opaque:%d, flag:%d, remark:%s, "
      "headLen:%d, bodyLen:%d ",
      code, language.c_str(), version, opaque, flag, remark.c_str(), headLen, bodyLen);
  RemotingCommand* cmd = new RemotingCommand(code, language, version, opaque, flag, remark, NULL);
  cmd->setParsedJson(object);
  if (bodyLen > 0) {
    cmd->SetBody(pData + 4 + headLen, bodyLen);
  }
  return cmd;
}

void RemotingCommand::markResponseType() {
  int bits = 1 << RPC_TYPE;
  m_flag |= bits;
}

bool RemotingCommand::isResponseType() {
  int bits = 1 << RPC_TYPE;
  return (m_flag & bits) == bits;
}

void RemotingCommand::markOnewayRPC() {
  int bits = 1 << RPC_ONEWAY;
  m_flag |= bits;
}

bool RemotingCommand::isOnewayRPC() {
  int bits = 1 << RPC_ONEWAY;
  return (m_flag & bits) == bits;
}

void RemotingCommand::setOpaque(const int opa) {
  m_opaque = opa;
}

void RemotingCommand::SetExtHeader(int code) {
  try {
    Json::Value ext = m_parsedJson["extFields"];
    if (!ext.isNull()) {
      m_pExtHeader = NULL;
      switch (code) {
        case SEND_MESSAGE:
          m_pExtHeader.reset(SendMessageResponseHeader::Decode(ext));
          break;
        case PULL_MESSAGE:
          m_pExtHeader.reset(PullMessageResponseHeader::Decode(ext));
          break;
        case GET_MIN_OFFSET:
          m_pExtHeader.reset(GetMinOffsetResponseHeader::Decode(ext));
          break;
        case GET_MAX_OFFSET:
          m_pExtHeader.reset(GetMaxOffsetResponseHeader::Decode(ext));
          break;
        case SEARCH_OFFSET_BY_TIMESTAMP:
          m_pExtHeader.reset(SearchOffsetResponseHeader::Decode(ext));
          break;
        case GET_EARLIEST_MSG_STORETIME:
          m_pExtHeader.reset(GetEarliestMsgStoretimeResponseHeader::Decode(ext));
          break;
        case QUERY_CONSUMER_OFFSET:
          m_pExtHeader.reset(QueryConsumerOffsetResponseHeader::Decode(ext));
          break;
        case RESET_CONSUMER_CLIENT_OFFSET:
          m_pExtHeader.reset(ResetOffsetRequestHeader::Decode(ext));
          break;
        case GET_CONSUMER_RUNNING_INFO:
          m_pExtHeader.reset(GetConsumerRunningInfoRequestHeader::Decode(ext));
          break;
        case NOTIFY_CONSUMER_IDS_CHANGED:
          m_pExtHeader.reset(NotifyConsumerIdsChangedRequestHeader::Decode(ext));
          break;
        case CHECK_TRANSACTION_STATE:
          m_pExtHeader.reset(CheckTransactionStateRequestHeader::Decode(ext));
          break;
        default:
          break;
      }
    }
  } catch (std::exception& e) {
    //LOG_ERROR("set response head error");
  }
}

void RemotingCommand::setCode(int code) {
  m_code = code;
}

int RemotingCommand::getCode() const {
  return m_code;
}

int RemotingCommand::getOpaque() const {
  return m_opaque;
}

string RemotingCommand::getRemark() const {
  return m_remark;
}

void RemotingCommand::setRemark(string mark) {
  m_remark = mark;
}

CommandHeader* RemotingCommand::getCommandHeader() const {
  return m_pExtHeader.get();
}

void RemotingCommand::setParsedJson(Json::Value json) {
  m_parsedJson = json;
}

const int RemotingCommand::getFlag() const {
  return m_flag;
}

const int RemotingCommand::getVersion() const {
  return m_version;
}

void RemotingCommand::setMsgBody(const string& body) {
  m_msgBody = body;
}

string RemotingCommand::getMsgBody() const {
  return m_msgBody;
}

void RemotingCommand::addExtField(const string& key, const string& value) {
  m_extFields[key] = value;
}

std::string RemotingCommand::ToString() const {
  std::stringstream ss;
  ss << "code:" << m_code << ",opaque:" << m_opaque << ",flag:" << m_flag << ",body.size:" << m_body.GetSize()
     << ",header.size:" << m_head.GetSize();
  return ss.str();
}

}//    end of lxmq

