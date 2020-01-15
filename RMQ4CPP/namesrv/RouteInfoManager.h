/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/
#ifndef __ROUTEINFO_MANAGER__H_
#define __ROUTEINFO_MANAGER__H_

#include <unordered_map>
using namespace std;

namespace lxmq{

class RouteInfoManager
{
public:
	RouteInfoManager();
	~RouteInfoManager();

	void registerBroker(const String clusterName,
        const String brokerAddr,
        const String brokerName,
        const long brokerId,
        const String haServerAddr,
        const List<String> filterServerList);
private:
    const static long BROKER_CHANNEL_EXPIRED_TIME = 1000 * 60 * 2;
    const mutux lock;
    const unordered_map<String/* topic */, List<QueueData>> topicQueueTable;
    const unordered_map<String/* brokerName */, BrokerData> brokerAddrTable;
    const unordered_map<String/* clusterName */, Set<String/* brokerName */>> clusterAddrTable;
    const unordered_map<String/* brokerAddr */, BrokerLiveInfo> brokerLiveTable;
    const unordered_map<String/* brokerAddr */, List<String>/* Filter Server */> filterServerTable;


};

}//    end of lxmq



#endif //__ROUTEINFO_MANAGER__H_

