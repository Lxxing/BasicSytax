
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
	struct Demo{
		int a;//4,offset 0
		char b;//1,offset 4
		double c;//8,offset 6
		char d[];//1,offset 14
	};
#pragma pack(pop) // 恢复先前的pack设置

	typedef struct
	{
		long  l;
		short s;

	} SBAR;

	typedef struct
	{
		int   i;
		float f;
		SBAR  b;

	} SFOO;
	/* Output is compiler dependent */
	
	/////unordered_map test///
	const string hl = "hello";
	unordered_map<string, string> unmap;
	unordered_map<string, string>::iterator it = unmap.find(hl);

	if (it == unmap.end())
	{
		unmap.insert(std::make_pair(hl, "word"));
	}

	for (auto& x : unmap)
		std::cout << x.first << ": " << x.second << std::endl;


	unordered_map<string, set<string>> setmap;

	unordered_map<string, set<string>>::iterator setit = setmap.find(hl);

	if (setit == setmap.end())
	{
		set<string> tmp;
		tmp.insert("set");
		setmap.insert(std::make_pair(hl, tmp));
	}

	for (auto& x : setmap)
	{
		std::cout << x.first << ": "  <<std::endl;
		for (auto& y : x.second)
		{
			std::cout << y << std::endl;
		}
	}
		


	///////////
	struct PA patest;

	char tmp[255] = "/tests/a/00000000000000000000257";

	char *p = p = strrchr(tmp, '/');
	__int64 value = 0;
	//sscanf(p, "%lld", &value);
	
	struct PA *pata = (struct PA *)malloc(sizeof(struct PA));
	testPa(&pata);
	patest = *pata;
	char * b = (char*)malloc(10);
	char * a = (char*)malloc(10);
	memcpy(b,a,10);
	int i = 0x1234;
	printf("Offset of 'l' is %u \n", offsetof(SFOO, b.l));

	printf("sizeof(struct Demo)=%ld\n", (long) sizeof(struct Demo));
	printf("offsets: a=%ld; b=%ld; c=%ld d=%ld\n",
		(long)offsetof(struct Demo, a),
		(long)offsetof(struct Demo, b),
		(long)offsetof(struct Demo, c),
		(long)offsetof(struct Demo, d));

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
