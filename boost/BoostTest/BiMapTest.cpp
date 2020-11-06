#include "BiMapTest.h"

#include <boost/bimap.hpp>
#include <iostream>

BiMapTest::BiMapTest()
{
	typedef boost::bimap< int, std::string > bm_type;
	bm_type bm;

	bm.insert(bm_type::value_type(1, "one"));
	bm.insert(bm_type::value_type(2, "two"));

	std::cout << "There are " << bm.size() << "relations" << std::endl;

	for (bm_type::const_iterator iter = bm.begin(), iend = bm.end();
		iter != iend; ++iter)
	{
		// iter->left  : data : int
		// iter->right : data : std::string

		std::cout << iter->left << " <--> " << iter->right << std::endl;
	}
}
