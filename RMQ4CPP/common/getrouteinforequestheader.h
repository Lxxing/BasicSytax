/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#ifndef __GETROUTEINFO_REQUESTHEADER__H_
#define __GETROUTEINFO_REQUESTHEADER__H_


#include "CommandCustomHeader.h"


namespace lxmq{

class GetRouteInfoRequestHeader : public CommandCustomHeader
{
public:
	void checkFields() {}

	string getTopic() {
        return topic;
    }

    void setTopic(string topic) {
        this->topic = topic;
    }
	static CommandCustomHeader* decodeCommandCustomHeader(Json::Value &ext)
	{
		GetRouteInfoRequestHeader* rnt = new GetRouteInfoRequestHeader();
		Json::Value& tempValue = ext["topic"];
		if (tempValue.isString()) {
			rnt->topic = tempValue.asString();
		}

		return rnt;
	}

private:
	string topic;


};

}//    end of lxmq

#endif //__GETROUTEINFO_REQUESTHEADER__H_

