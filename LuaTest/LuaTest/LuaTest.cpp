#include <lua.hpp>
#include <iostream>  
#include <string.h>  
using namespace std;

void main()
{
	//1.创建Lua状态  
	lua_State *L = luaL_newstate();
	if (L == NULL)
	{
		return;
	}

	//2.加载Lua文件  
	int bRet = luaL_loadfile(L, "D:\\Code\\lua-5.3.4\\lib\\hello.lua");
	if (bRet)
	{
		cout << "load file error" << endl;
		cout << lua_tostring(L, -1) << endl;
		return;
	}

	//3.运行Lua文件  
	bRet = lua_pcall(L, 0, 0, 0);
	if (bRet)
	{
		cout << "pcall error" << endl;
		return;
	}

	//4.读取变量  
	lua_getglobal(L, "width");
	lua_getglobal(L, "heigth");
	lua_getglobal(L, "Window.Length");

	if (lua_isnumber(L, -3))        //取值输出  
	{
		double fValue = lua_tonumber(L, -2);
		cout << "width is " << fValue << endl;
	}
	if (lua_isnumber(L, -2))        //取值输出  
	{
		double fValue = lua_tonumber(L, -1);
		cout << "heigth is " << fValue << endl;
	}
	if (lua_isnumber(L, -1))        //取值输出  
	{
		double fValue = lua_tonumber(L, -1);
		cout << "Legth is " << fValue << endl;
	}
	//7.关闭state  
	lua_close(L);
	return;



}