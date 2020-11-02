#ifndef __NRVO_TEST_H__
#define __NRVO_TEST_H__

struct Sub
{
	Sub()
	  :index(1)
	{}

	int index;
};

Sub & TestSub()
{
	Sub rnt = Sub();
	rnt.index = 3;
	return rnt;
}




#endif __NRVO_TEST_H__
