
// Sytax.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <unordered_map>
#include <set>
using namespace std;

#define LenTest 20
int TestArray[LenTest];

struct PA
{
	void *pa;
};

void testPa(struct PA**pp)
{
	(*pp)->pa = (void*)malloc(1);
}
int main()
{
#pragma pack(push) // 将当前pack设置压栈保存
#pragma pack(2)// 必须在结构体定义之前使用
	struct Demo {
		int a;//4,offset 0
		char b;//1,offset 4
		double c;//8,offset 6
		char d[];//1,offset 14
	};
#pragma pack(pop) // 恢复先前的pack设置

#define CF_NAME_LEN 47
	typedef struct
	{
		char type;
		char* pp;
		int  brokerId;
		char brokerDesc[CF_NAME_LEN + 1];
		char hostName[CF_NAME_LEN + 1];

	} BrokerConfig;
	/* Output is compiler dependent */

	printf("sizeof(struct Demo)=%ld\n", (long) sizeof(struct Demo));
	printf("offsets: a=%ld; b=%ld; c=%ld d=%ld\n",
		(long)offsetof(struct Demo, a),
		(long)offsetof(struct Demo, b),
		(long)offsetof(struct Demo, c),
		(long)offsetof(struct Demo, d));

	printf("sizeof(struct BrokerConfig)=%ld\n", (long) sizeof(BrokerConfig));
	printf("offsets: a=%ld; b=%ld; c=%ld \n",
		(long)offsetof(BrokerConfig, brokerId),
		(long)offsetof(BrokerConfig, brokerDesc),
		(long)offsetof(BrokerConfig, hostName)
		);


	exit(EXIT_SUCCESS);

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
