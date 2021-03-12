// ConsoleTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

int main(int argc,char**argv)
{
	for (int i =0;i < argc;i++)
	{
		std::cout << argv[i]<<"\n";
	}
    std::cout << "Hello World!\n";
	std::cin.get();
}
