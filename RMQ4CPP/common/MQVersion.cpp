/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#include "MQVersion.h"

#define Enum2Sring(x) #x


namespace lxmq{


int MQVersion::s_CurrentVersion = MQVersion::V4_5_2;

const char* MQVersion::getVersionDesc(int value) {
  return Enum2Sring(MQVersion(value));
}

static MQVersion value2Version(int value)
{
	return (MQVersion)value;
}


}//    end of lxmq

