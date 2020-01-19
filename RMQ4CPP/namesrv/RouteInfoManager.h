/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* ��ӭ����ָ�� xiaoxiaomiantianwo@163.com
******************************************************************************************/
#ifndef __ROUTEINFO_MANAGER__H_
#define __ROUTEINFO_MANAGER__H_

#include <unordered_map>
#include <mutex>
#include <set>
using namespace std;

namespace lxmq{



class BrokerLiveInfo
{
public:
    long getLastUpdateTimestamp() {
        return lastUpdateTimestamp;
    }

    void setLastUpdateTimestamp(long lastUpdateTimestamp) {
        this->lastUpdateTimestamp = lastUpdateTimestamp;
    }

    string getHaServerAddr() {
        return haServerAddr;
    }

    void setHaServerAddr(string haServerAddr) {
        this->haServerAddr = haServerAddr;
    }

private:

    long lastUpdateTimestamp;
    string haServerAddr;

};


class RouteInfoManager
{
public:
	RouteInfoManager();
	~RouteInfoManager();

	void registerBroker(const string clusterName,
        const string brokerAddr,
        const string brokerName,
        const long brokerId,
        const string haServerAddr);
private:
    const static long BROKER_CHANNEL_EXPIRED_TIME = 1000 * 60 * 2;
    const mutex lock;
    //const unordered_map<string/* topic */, list<QueueData>> topicQueueTable;
    //const unordered_map<string/* brokerName */, BrokerData> brokerAddrTable;
    const unordered_map<string/* clusterName */, set<string/* brokerName */>> clusterAddrTable;
    const unordered_map<string/* brokerAddr */, BrokerLiveInfo> brokerLiveTable;
    const unordered_map<string/* brokerAddr */, list<string>/* Filter Server */> filterServerTable;


};




}//    end of lxmq



#endif //__ROUTEINFO_MANAGER__H_

