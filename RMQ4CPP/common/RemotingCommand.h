/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/
	
#ifndef __REMOTINGCOMMAND_H__
#define __REMOTINGCOMMAND_H__

#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <memory>
#include <sstream>

#include "MQBuffer.h"
#include "RemotingUtil.h"
#include "CommandHeader.h"
using namespace std;

namespace lxmq{

//<!***************************************************************************
const int RPC_TYPE = 0;    // 0, REQUEST_COMMAND // 1, RESPONSE_COMMAND;
const int RPC_ONEWAY = 1;  // 0, RPC // 1, Oneway;
//<!***************************************************************************
class RemotingCommand {
 public:
  RemotingCommand() : m_code(0){};
  RemotingCommand(int code) {};
  RemotingCommand(const RemotingCommand& command) {};
  
RemotingCommand::RemotingCommand(int code,
                                 string language,
                                 int version,
                                 int opaque,
                                 int flag,
                                 string remark,
                                 CommandHeader* pExtHeader)
    : m_code(code),
      m_language(language),
      m_version(version),
      m_opaque(opaque),
      m_flag(flag),
      m_remark(remark),
      m_pExtHeader(pExtHeader) {}
  RemotingCommand& operator=(const RemotingCommand& command);
  virtual ~RemotingCommand();
  const MQBuffer* GetHead() const;
  const MQBuffer* GetBody() const;

  void SetBody(const char* pData, int len);
  void setOpaque(const int opa);
  void SetExtHeader(int code);
  void setCode(int code);
  int getCode() const;
  int getOpaque() const;
  void setRemark(string mark);
  string getRemark() const;
  void markResponseType();
  bool isResponseType();
  void markOnewayRPC();
  bool isOnewayRPC();
  Json::Value& getParsedJson();
  void setParsedJson(Json::Value json);
  CommandHeader* getCommandHeader() const;
  const int getFlag() const;
  const int getVersion() const;
  void addExtField(const string& key, const string& value);
  string getMsgBody() const;
  void setMsgBody(const string& body);

 public:
  void Encode();
  RemotingCommand decodeCommandCustomHeader(   RemotingCommand request);
  static RemotingCommand* Decode(const MQBuffer& mem);
  std::string ToString() const;

 private:
  void Assign(const RemotingCommand& command);

 private:
  int m_code;
  string m_language;
  int m_version;
  int m_opaque;
  int m_flag;
  string m_remark;
  string m_msgBody;
  map<string, string> m_extFields;

  MQBuffer m_head;
  MQBuffer m_body;
  //<!save here
  Json::Value m_parsedJson;
  unique_ptr<CommandHeader> m_pExtHeader;

};

}//    end of lxmq


#endif //__REMOTINGCOMMAND_H__

