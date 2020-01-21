/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/
#ifndef __REGISTER_BROKERBODY__H_
#define __REGISTER_BROKERBODY__H_

namespace lxmq{

class RegisterBrokerBody
{
public:
	RegisterBrokerBody();
	~RegisterBrokerBody();

	static RegisterBrokerBody decode();

};

}//    end of lxmq




#endif //__REGISTER_BROKERBODY__H_

