/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* ��ӭ����ָ�� xiaoxiaomiantianwo@163.com
******************************************************************************************/

#ifndef __DEFAULTREQUEST_PROCESSOR__H_
#define __DEFAULTREQUEST_PROCESSOR__H_

#include "RequestProcessor.h"

namespace lxmq{

class NamesrvController;
class DefaultRequestProcessor : public RequestProcessor
{
public:
	DefaultRequestProcessor(NamesrvController *);
	~DefaultRequestProcessor();
	
	RemotingCommand processRequest(const MQBuffer & msg);

	RemotingCommand registerBroker(RemotingCommand *request);

private:
	NamesrvController *namesrvController;
};

}//    end of lxmq



#endif //__DEFAULTREQUEST_PROCESSOR__H_

