#include <iostream>
using namespace std;

#include <boost/shared_ptr.hpp>  
#include <boost/make_shared.hpp>
#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>

#include "NRVO.h"
#include "NRVO_test.h"

class CBase
{
public:
	CBase() { }
	virtual ~CBase() { }

	void myBase()
	{
		cout << "CBase::myBase" << endl;
	}
};

struct base
{

	int a;
};

class derive :public base
{
public:
	int b;

};

class CDerive : public CBase
{
public:
	CDerive() { }
	~CDerive() { }

	void myDerive()
	{
		cout << "CDerive::myDerive" << endl;
	}
};

int main(void)
{
#define BOOST_DATE_TIME_HAS_HIGH_PRECISION_CLOCK
	boost::posix_time::ptime timeun = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::ptime timelc = boost::posix_time::microsec_clock::local_time();

	std::cout << "untime:" << boost::posix_time::to_simple_string(timeun) << std::endl;
	std::cout << "lctime:" << boost::posix_time::to_simple_string(timelc) << std::endl;
	Sub &sub = TestSub();

	int refindex = sub.index;

	A a;
	a = MyMethod();

	//上行的转换（派生类到基类的转换）  
	boost::shared_ptr<CDerive> spDeriveUp;
	boost::shared_ptr<CBase> spBaseUp;

	boost::shared_ptr<derive> dir = boost::make_shared<derive>();
	dir->a = sub.index;
	dir->b = 11;
	base * tmp = static_cast<base*>(dir.get());


	spDeriveUp = boost::make_shared<CDerive>();
	spBaseUp = boost::dynamic_pointer_cast<CBase>(spDeriveUp);
	spBaseUp->myBase();

	//下行的转换（基类到派生类的转换)  
	boost::shared_ptr<CDerive> spDeriveDown;
	boost::shared_ptr<CBase> spBaseDown;
	spBaseDown = boost::make_shared<CBase>();
	spDeriveDown = boost::dynamic_pointer_cast<CDerive>(spBaseDown);
	if (spDeriveDown == NULL)   //由于会进行类型的检查，这边是返回NULL  
		cout << "spDeriveDown is null" << endl;

	return 0;
}