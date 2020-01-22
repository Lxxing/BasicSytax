/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/
#include <unordered_map>
#include <set>

#include "RouteInfoManager.h"
#include "TopicRouteData.h"

namespace lxmq{

RouteInfoManager::RouteInfoManager()
{

}

RouteInfoManager::~RouteInfoManager()
{

}

void RouteInfoManager::registerBroker(const string clusterName, 
	const string brokerAddr, const string brokerName,
	const long brokerId, const string haServerAddr)
{
	std::lock_guard<std::mutex> guard(rwlock);

	ClusterBroker::iterator bs = clusterAddrTable.find(clusterName);
	if(bs == clusterAddrTable.end())
	{
		set<string> brokerNames;
		brokerNames.insert(brokerName);
		clusterAddrTable.insert(std::make_pair(clusterName,brokerNames));
	}

	bool registerFirst = false;

	BrokerDatas::iterator britor = brokerAddrTable.find(brokerName);
    if (brokerAddrTable.end() == britor) 
    {
    	registerFirst = true;
        BrokerData brokerData;
        brokerData.brokerName = brokerName;
        brokerAddrTable.insert(make_pair(brokerName, brokerData));
    }

    britor = brokerAddrTable.find(brokerName);
    std::map<int, string> &brokerAddrs = britor->second.brokerAddrs;
    int brokeraddrsnum = brokerAddrs.size();
    std::map<int, string>::iterator pbegin = brokerAddrs.begin();
	for (; pbegin != brokerAddrs.end(); )
	{
		if("" == brokerAddr && (brokerAddr == pbegin->second)
			&& brokerId != pbegin->first)
		{
			brokerAddrs.erase(pbegin++);
			break;
		}
		else
		{
			pbegin++;
		}
	}

    brokerAddrs.insert(make_pair(brokerId,brokerAddr));
	
	if (MixAll::MASTER_ID == brokerId)
	{
		//
	}
	else
	{
		//
	}

    
}


TopicRouteData RouteInfoManager::pickupTopicRouteData(const string topic)
{
	TopicRouteData topicRouteData;
    bool foundQueueData = false;
    bool foundBrokerData = false;
    set<string> brokerNameSet;
    list<BrokerData> brokerDataList;
    //topicRouteData.setBrokerDatas(brokerDataList);

	std::lock_guard<std::mutex> guard(rwlock);

	TopicQueue::iterator tpq = topicQueueTable.find(topic);
	if (tpq != topicQueueTable.end())
	{
		
	}

	return topicRouteData;
}




}//    end of lxmq

