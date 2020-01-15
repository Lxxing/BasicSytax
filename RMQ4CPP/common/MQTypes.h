/******************************************************************************************
* RMQ4CPP 
* Auther : luxiang
* github:https://github.com/Lxxing?tab=repositories
* 欢迎交流指正 xiaoxiaomiantianwo@163.com
******************************************************************************************/

#ifndef __MQTYPES__H_
#define __MQTYPES__H_
	
#ifdef _WIN32
#include <Winsock2.h>
#include <Windows.h>
#endif

#include <boost/cstdint.hpp>


using MQUINT8 = boost::uint8_t;
using MQINT8 = boost::int8_t;

using MQUINT16 = boost::uint16_t;
using MQINT16 = boost::int16_t;

using MQUINT32 = boost::uint32_t;
using MQINT32 = boost::int32_t;

using MQUINT64 = boost::uint64_t;
using MQINT64 = boost::int64_t;	
	
	
#endif //__MQTYPES__H_

