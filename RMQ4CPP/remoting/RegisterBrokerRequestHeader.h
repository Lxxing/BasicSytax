/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/


#ifndef __REGISTERBOKER_REQUESTHEADER__H_
#define __REGISTERBOKER_REQUESTHEADER__H_

#include "CommandCustomHeader.h"
	
namespace lxmq{

class RegisterBrokerRequestHeader : public CommandCustomHeader
{
public:

    void checkFields() {}
    
    string getBrokerName() {
        return brokerName;
    }

    void setBrokerName(string brokerName) {
        this->brokerName = brokerName;
    }

    string getBrokerAddr() {
        return brokerAddr;
    }

    void setBrokerAddr(string brokerAddr) {
        this->brokerAddr = brokerAddr;
    }

    string getClusterName() {
        return clusterName;
    }

    void setClusterName(string clusterName) {
        this->clusterName = clusterName;
    }

    string getHaServerAddr() {
        return haServerAddr;
    }

    void setHaServerAddr(string haServerAddr) {
        this->haServerAddr = haServerAddr;
    }

    long getBrokerId() {
        return brokerId;
    }

    void setBrokerId(long brokerId) {
        this->brokerId = brokerId;
    }

    bool isCompressed() {
        return compressed;
    }

    void setCompressed(bool compressed) {
        this->compressed = compressed;
    }

    int getBodyCrc32() {
        return bodyCrc32;
    }

    void setBodyCrc32(int bodyCrc32) {
        this->bodyCrc32 = bodyCrc32;
    }
private:
    string brokerName;
    string brokerAddr;
    string clusterName;
    string haServerAddr;
    long brokerId;
    bool compressed;

    int bodyCrc32 = 0;
};


}//    end of lxmq
	
	
	
	
#endif //__REGISTERBOKER_REQUESTHEADER__H_


