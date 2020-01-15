/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#ifndef __COMMANDCUSTOMHEADER_H__
#define __COMMANDCUSTOMHEADER_H__

#include <map>
#include <string>
#include <iostream>
using namespace std;
#include "json/json.h"

#include "MQTypes.h"
#include "MQBuffer.h"

namespace lxmq{

class CommandHeader {
 public:
  virtual ~CommandHeader() {}
  virtual void Encode(Json::Value& outData) {}
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap) {}
};

class CheckTransactionStateRequestHeader : public CommandHeader {
 public:
  CheckTransactionStateRequestHeader() {}
  CheckTransactionStateRequestHeader(long tableOffset,
                                     long commLogOffset,
                                     const std::string& msgid,
                                     const std::string& transactionId,
                                     const std::string& offsetMsgId)
      : m_tranStateTableOffset(tableOffset),
        m_commitLogOffset(commLogOffset),
        m_msgId(msgid),
        m_transactionId(transactionId),
        m_offsetMsgId(offsetMsgId) {}
  virtual ~CheckTransactionStateRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  static CommandHeader* Decode(Json::Value& ext);
  virtual void SetDeclaredFieldOfCommandHeader(std::map<std::string, std::string>& requestMap);
  std::string toString();

 public:
  long m_tranStateTableOffset;
  long m_commitLogOffset;
  std::string m_msgId;
  std::string m_transactionId;
  std::string m_offsetMsgId;
};

class EndTransactionRequestHeader : public CommandHeader {
 public:
  EndTransactionRequestHeader() {}
  EndTransactionRequestHeader(const std::string& groupName,
                              long tableOffset,
                              long commLogOffset,
                              int commitOrRoll,
                              bool fromTransCheck,
                              const std::string& msgid,
                              const std::string& transId)
      : m_producerGroup(groupName),
        m_tranStateTableOffset(tableOffset),
        m_commitLogOffset(commLogOffset),
        m_commitOrRollback(commitOrRoll),
        m_fromTransactionCheck(fromTransCheck),
        m_msgId(msgid),
        m_transactionId(transId) {}
  virtual ~EndTransactionRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(std::map<string, string>& requestMap);
  std::string toString();

 public:
  std::string m_producerGroup;
  long m_tranStateTableOffset;
  long m_commitLogOffset;
  int m_commitOrRollback;
  bool m_fromTransactionCheck;
  std::string m_msgId;
  std::string m_transactionId;
};

//<!************************************************************************
class GetRouteInfoRequestHeader : public CommandHeader {
 public:
  GetRouteInfoRequestHeader(const string& top) : topic(top) {}
  virtual ~GetRouteInfoRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 private:
  string topic;
};

//<!************************************************************************
class UnregisterClientRequestHeader : public CommandHeader {
 public:
  UnregisterClientRequestHeader(string cID, string proGroup, string conGroup)
      : clientID(cID), producerGroup(proGroup), consumerGroup(conGroup) {}
  virtual ~UnregisterClientRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 private:
  string clientID;
  string producerGroup;
  string consumerGroup;
};

//<!************************************************************************
class CreateTopicRequestHeader : public CommandHeader {
 public:
  CreateTopicRequestHeader() : readQueueNums(0), writeQueueNums(0), perm(0) {}
  virtual ~CreateTopicRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string topic;
  string defaultTopic;
  int readQueueNums;
  int writeQueueNums;
  int perm;
  string topicFilterType;
};

//<!************************************************************************
class SendMessageRequestHeader : public CommandHeader {
 public:
  SendMessageRequestHeader()
      : defaultTopicQueueNums(0),
        queueId(0),
        sysFlag(0),
        bornTimestamp(0),
        flag(0),
        reconsumeTimes(0),
        unitMode(false),
        batch(false) {}
  virtual ~SendMessageRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);
  int getReconsumeTimes();
  void setReconsumeTimes(int input_reconsumeTimes);

 public:
  string producerGroup;
  string topic;
  string defaultTopic;
  int defaultTopicQueueNums;
  int queueId;
  int sysFlag;
  MQINT64 bornTimestamp;
  int flag;
  string properties;
  int reconsumeTimes;
  bool unitMode;
  bool batch;
};

//<!************************************************************************
class SendMessageResponseHeader : public CommandHeader {
 public:
  SendMessageResponseHeader() : queueId(0), queueOffset(0) { msgId.clear(); }
  virtual ~SendMessageResponseHeader() {}
  static CommandHeader* Decode(Json::Value& ext);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string msgId;
  int queueId;
  MQINT64 queueOffset;
};

//<!************************************************************************
class PullMessageRequestHeader : public CommandHeader {
 public:
  PullMessageRequestHeader()
      : queueId(0),
        maxMsgNums(0),
        sysFlag(0),
        queueOffset(0),
        commitOffset(0),
        suspendTimeoutMillis(0),
        subVersion(0) {}
  virtual ~PullMessageRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string consumerGroup;
  string topic;
  int queueId;
  int maxMsgNums;
  int sysFlag;
  string subscription;
  MQINT64 queueOffset;
  MQINT64 commitOffset;
  MQINT64 suspendTimeoutMillis;
  MQINT64 subVersion;
};

//<!************************************************************************
class PullMessageResponseHeader : public CommandHeader {
 public:
  PullMessageResponseHeader() : suggestWhichBrokerId(0), nextBeginOffset(0), minOffset(0), maxOffset(0) {}
  virtual ~PullMessageResponseHeader() {}
  static CommandHeader* Decode(Json::Value& ext);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  MQINT64 suggestWhichBrokerId;
  MQINT64 nextBeginOffset;
  MQINT64 minOffset;
  MQINT64 maxOffset;
};

//<!************************************************************************
class GetConsumerListByGroupResponseHeader : public CommandHeader {
 public:
  GetConsumerListByGroupResponseHeader() {}
  virtual ~GetConsumerListByGroupResponseHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);
};

//<!***************************************************************************
class GetMinOffsetRequestHeader : public CommandHeader {
 public:
  GetMinOffsetRequestHeader() : queueId(0){};
  virtual ~GetMinOffsetRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string topic;
  int queueId;
};

//<!***************************************************************************
class GetMinOffsetResponseHeader : public CommandHeader {
 public:
  GetMinOffsetResponseHeader() : offset(0){};
  virtual ~GetMinOffsetResponseHeader() {}
  static CommandHeader* Decode(Json::Value& ext);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  MQINT64 offset;
};

//<!***************************************************************************
class GetMaxOffsetRequestHeader : public CommandHeader {
 public:
  GetMaxOffsetRequestHeader() : queueId(0){};
  virtual ~GetMaxOffsetRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string topic;
  int queueId;
};

//<!***************************************************************************
class GetMaxOffsetResponseHeader : public CommandHeader {
 public:
  GetMaxOffsetResponseHeader() : offset(0){};
  virtual ~GetMaxOffsetResponseHeader() {}
  static CommandHeader* Decode(Json::Value& ext);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  MQINT64 offset;
};

//<!***************************************************************************
class SearchOffsetRequestHeader : public CommandHeader {
 public:
  SearchOffsetRequestHeader() : queueId(0), timestamp(0){};
  virtual ~SearchOffsetRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string topic;
  int queueId;
  MQINT64 timestamp;
};

//<!***************************************************************************
class SearchOffsetResponseHeader : public CommandHeader {
 public:
  SearchOffsetResponseHeader() : offset(0){};
  virtual ~SearchOffsetResponseHeader() {}
  static CommandHeader* Decode(Json::Value& ext);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  MQINT64 offset;
};

//<!***************************************************************************
class ViewMessageRequestHeader : public CommandHeader {
 public:
  ViewMessageRequestHeader() : offset(0){};
  virtual ~ViewMessageRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  MQINT64 offset;
};

//<!***************************************************************************
class GetEarliestMsgStoretimeRequestHeader : public CommandHeader {
 public:
  GetEarliestMsgStoretimeRequestHeader() : queueId(0){};
  virtual ~GetEarliestMsgStoretimeRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string topic;
  int queueId;
};

//<!***************************************************************************
class GetEarliestMsgStoretimeResponseHeader : public CommandHeader {
 public:
  GetEarliestMsgStoretimeResponseHeader() : timestamp(0){};
  virtual ~GetEarliestMsgStoretimeResponseHeader() {}
  static CommandHeader* Decode(Json::Value& ext);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  MQINT64 timestamp;
};

//<!***************************************************************************
class GetConsumerListByGroupRequestHeader : public CommandHeader {
 public:
  GetConsumerListByGroupRequestHeader(){};
  virtual ~GetConsumerListByGroupRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string consumerGroup;
};

//<!************************************************************************
class QueryConsumerOffsetRequestHeader : public CommandHeader {
 public:
  QueryConsumerOffsetRequestHeader() : queueId(0){};
  virtual ~QueryConsumerOffsetRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string consumerGroup;
  string topic;
  int queueId;
};

//<!************************************************************************
class QueryConsumerOffsetResponseHeader : public CommandHeader {
 public:
  QueryConsumerOffsetResponseHeader() : offset(0){};
  virtual ~QueryConsumerOffsetResponseHeader() {}
  static CommandHeader* Decode(Json::Value& ext);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  MQINT64 offset;
};

//<!************************************************************************
class UpdateConsumerOffsetRequestHeader : public CommandHeader {
 public:
  UpdateConsumerOffsetRequestHeader() : queueId(0), commitOffset(0){};
  virtual ~UpdateConsumerOffsetRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string consumerGroup;
  string topic;
  int queueId;
  MQINT64 commitOffset;
};

//<!***************************************************************************
class ConsumerSendMsgBackRequestHeader : public CommandHeader {
 public:
  ConsumerSendMsgBackRequestHeader() : delayLevel(0), offset(0){};
  virtual ~ConsumerSendMsgBackRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  string group;
  int delayLevel;
  MQINT64 offset;
};

//<!***************************************************************************
class GetConsumerListByGroupResponseBody {
 public:
  GetConsumerListByGroupResponseBody(){};
  virtual ~GetConsumerListByGroupResponseBody() {}
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);

 public:
  static void Decode(const MQBuffer* mem, vector<string>& cids);
};

class ResetOffsetRequestHeader : public CommandHeader {
 public:
  ResetOffsetRequestHeader() {}
  ~ResetOffsetRequestHeader() {}
  static CommandHeader* Decode(Json::Value& ext);
  void setTopic(const string& tmp);
  void setGroup(const string& tmp);
  void setTimeStamp(const MQINT64& tmp);
  void setForceFlag(const bool& tmp);
  const string getTopic() const;
  const string getGroup() const;
  const MQINT64 getTimeStamp() const;
  const bool getForceFlag() const;

 private:
  string topic;
  string group;
  MQINT64 timestamp;
  bool isForce;
};

class GetConsumerRunningInfoRequestHeader : public CommandHeader {
 public:
  GetConsumerRunningInfoRequestHeader() {}
  virtual ~GetConsumerRunningInfoRequestHeader() {}
  virtual void Encode(Json::Value& outData);
  virtual void SetDeclaredFieldOfCommandHeader(map<string, string>& requestMap);
  static CommandHeader* Decode(Json::Value& ext);
  const string getConsumerGroup() const;
  void setConsumerGroup(const string& consumerGroup);
  const string getClientId() const;
  void setClientId(const string& clientId);
  const bool isJstackEnable() const;
  void setJstackEnable(const bool& jstackEnable);

 private:
  string consumerGroup;
  string clientId;
  bool jstackEnable;
};

class NotifyConsumerIdsChangedRequestHeader : public CommandHeader {
 public:
  NotifyConsumerIdsChangedRequestHeader() {}
  virtual ~NotifyConsumerIdsChangedRequestHeader() {}
  static CommandHeader* Decode(Json::Value& ext);
  void setGroup(const string& tmp);
  const string getGroup() const;

 private:
  string consumerGroup;
};

}//    end of lxmq

#endif
