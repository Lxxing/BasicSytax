/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/
#ifndef __REQUEST_PROCESSOR__H_
#define __REQUEST_PROCESSOR__H_

#include "MQBuffer.h"
#include "RemotingCommand.h"

namespace lxmq{


class RequestProcessor
{
public:

	virtual RemotingCommand processRequest(const MQBuffer & msg) = 0;


};


}//    end of lxmq


#endif //__REQUEST_PROCESSOR__H_

