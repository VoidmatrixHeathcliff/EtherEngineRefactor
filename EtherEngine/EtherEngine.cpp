#include "EtherEngine.h"

#include "Register.h"

SDL_Event event;

SDL_Window*   pGlobalWindow = nullptr;
SDL_Renderer* pGlobalRenderer = nullptr;

struct SDL_Point pointCursorPos;

#undef main
int main(int argc, char** argv)
{
	/*
		初始化 Lua 虚拟机
	*/
	lua_State* pGlobalLuaVM = luaL_newstate();
	luaL_openlibs(pGlobalLuaVM);
	lua_gc(pGlobalLuaVM, LUA_GCINC, 100);

	/*
		加载配置文件
	*/
	std::ifstream fConfigFile("config.json");
	if (!fConfigFile.good())
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Configure Error",
			"Failed to open configuration file: config.json",
			nullptr
		);
		return -1;
	}

	std::stringstream ssConfigContent;
	ssConfigContent << fConfigFile.rdbuf();
	fConfigFile.close(); fConfigFile.clear();

	// "entry": ""
	cJSON* pJSONConfigRoot = nullptr, * pJSONConfigEntry = nullptr;
	if (!((pJSONConfigRoot = cJSON_Parse(ssConfigContent.str().c_str()))
		&& (pJSONConfigRoot->type == cJSON_Object)))
	{
		ssConfigContent.clear();
		cJSON_Delete(pJSONConfigRoot);
		pJSONConfigRoot = nullptr;
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Configure Error",
			"Failed to parse configuration file",
			nullptr
		);
		return -1;
	}
	if (!((pJSONConfigEntry = cJSON_GetObjectItem(pJSONConfigRoot, "entry"))
		&& (pJSONConfigEntry->type == cJSON_String)))
	{
		ssConfigContent.clear();
		cJSON_Delete(pJSONConfigRoot);
		pJSONConfigRoot = nullptr;
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Configure Error",
			"Invalid entry file configuration",
			nullptr
		);
		return -1;
	}
	std::string strNameEntry = pJSONConfigEntry->valuestring;
	ssConfigContent.clear();

	// "package": []
	cJSON* pJSONConfigPackage = nullptr;
	if ((pJSONConfigPackage = cJSON_GetObjectItem(pJSONConfigRoot, "package")))
	{
		cJSON* pJSONConfigPath = nullptr, * pJSONConfigCPath = nullptr;
		// "path": []
		if ((pJSONConfigPath = cJSON_GetObjectItem(pJSONConfigPackage, "path"))
			&& (pJSONConfigPath->type == cJSON_Array))
		{
			lua_getglobal(pGlobalLuaVM, "package");
			lua_getfield(pGlobalLuaVM, -1, "path");
			std::string _strPath = lua_tostring(pGlobalLuaVM, -1);
			int _nArraySize = cJSON_GetArraySize(pJSONConfigPackage);
			for (int i = 0; i < _nArraySize; i++)
			{
				cJSON* _pJSONPathItem = nullptr;
				if ((_pJSONPathItem = cJSON_GetArrayItem(pJSONConfigPath, i))
					&& (_pJSONPathItem->type == cJSON_String))
					_strPath.append(";").append(_pJSONPathItem->valuestring);
			}
			lua_pushstring(pGlobalLuaVM, _strPath.c_str());
			lua_setfield(pGlobalLuaVM, -3, "path");
			lua_pop(pGlobalLuaVM, 2);
		}
		// "cpath": []
		if ((pJSONConfigCPath = cJSON_GetObjectItem(pJSONConfigPackage, "cpath"))
			&& pJSONConfigCPath->type == cJSON_Array)
		{
			lua_getglobal(pGlobalLuaVM, "package");
			lua_getfield(pGlobalLuaVM, -1, "cpath");
			std::string _strCPath = lua_tostring(pGlobalLuaVM, -1);
			int _nArraySize = cJSON_GetArraySize(pJSONConfigPackage);
			for (int i = 0; i < _nArraySize; i++)
			{
				cJSON* _pJSONPathItem = nullptr;
				if ((_pJSONPathItem = cJSON_GetArrayItem(pJSONConfigPath, i))
					&& (_pJSONPathItem->type == cJSON_String))
					_strCPath.append(";").append(_pJSONPathItem->valuestring);
			}
			lua_pushstring(pGlobalLuaVM, _strCPath.c_str());
			lua_setfield(pGlobalLuaVM, -3, "cpath");
			lua_pop(pGlobalLuaVM, 2);
		}
	}

	// "command": []
	std::vector<std::string> vCommandList;
	cJSON* pJSONConfigCommand = nullptr;
	if ((pJSONConfigCommand = cJSON_GetObjectItem(pJSONConfigRoot, "command"))
		&& (pJSONConfigCommand->type == cJSON_Array))
	{
		lua_newtable(pGlobalLuaVM);
		int _nArraySize = cJSON_GetArraySize(pJSONConfigCommand);
		for (int i = 0; i < _nArraySize; i++)
		{
			cJSON* _pJSONPathItem = nullptr;
			if ((_pJSONPathItem = cJSON_GetArrayItem(pJSONConfigCommand, i))
				&& (_pJSONPathItem->type == cJSON_String))
				vCommandList.push_back(_pJSONPathItem->valuestring);
		}
	}

	cJSON_Delete(pJSONConfigRoot);
	pJSONConfigRoot = nullptr, pJSONConfigEntry = nullptr;
	pJSONConfigPackage = nullptr, pJSONConfigCommand = nullptr;

	/*
		传递启动参数和环境变量
	*/
	lua_newtable(pGlobalLuaVM);
	for (int i = 0; i < argc; i++)
	{
		lua_pushinteger(pGlobalLuaVM, i + 1);
		lua_pushstring(pGlobalLuaVM, argv[i]);
		lua_settable(pGlobalLuaVM, -3);
	}
	for (int i = argc; i < vCommandList.size() + argc; i++)
	{
		lua_pushinteger(pGlobalLuaVM, i + 1);
		lua_pushstring(pGlobalLuaVM, vCommandList[i - argc].c_str());
		lua_settable(pGlobalLuaVM, -3);
	}
	lua_setglobal(pGlobalLuaVM, "_ARGV");
	std::vector<std::string>().swap(vCommandList);
	
	lua_newtable(pGlobalLuaVM);
	for (int i = 0; environ[i]; i++)
	{
		std::string strEnvp = environ[i];
		size_t nIndexEqual = strEnvp.find_first_of('=');
		lua_pushstring(pGlobalLuaVM, strEnvp.substr(0, nIndexEqual).c_str());
		lua_pushstring(pGlobalLuaVM, strEnvp.substr(nIndexEqual + 1).c_str());
		lua_settable(pGlobalLuaVM, -3);
	}
	lua_setglobal(pGlobalLuaVM, "_ENVP");

	/*
		设置 Built-in Package 数据
	*/
	std::vector<function<void()>> vecOnExitCallback;
	lua_getglobal(pGlobalLuaVM, "package");
	lua_getfield(pGlobalLuaVM, -1, "preload");
	for (const BuiltinPackageData& pkg : BuiltinPackageList)
	{
		lua_pushstring(pGlobalLuaVM, pkg.name.c_str());
		lua_pushcfunction(pGlobalLuaVM, pkg.on_load);
		lua_settable(pGlobalLuaVM, -3);
		if (pkg.on_exit) vecOnExitCallback.push_back(pkg.on_exit);
	}
	lua_pop(pGlobalLuaVM, 2);
	std::vector<BuiltinPackageData>().swap(BuiltinPackageList);

	/*
		设置引擎版本号
	*/
	lua_pushstring(pGlobalLuaVM, VERSION_ENGINE);
	lua_setglobal(pGlobalLuaVM, "_VERSION_ENGINE");

	/*
		初始化 SDL 支持
	*/
	SDL_Init(SDL_INIT_EVERYTHING);

	/*
		执行入口脚本文件
	*/
	try
	{
		if (luaL_dofile(pGlobalLuaVM, strNameEntry.c_str()))
			SDL_ShowSimpleMessageBox(
				SDL_MESSAGEBOX_ERROR,
				"Script Error",
				lua_tostring(pGlobalLuaVM, -1),
				pGlobalWindow
			);
	}
	catch (const std::exception& err)
	{
		SDL_ShowSimpleMessageBox(
			SDL_MESSAGEBOX_ERROR,
			"Engine Crashed",
			std::string("Underlying code exception: ").append(err.what()).c_str(),
			pGlobalWindow
		);
	}

	/*
		释放引擎资源
	*/
	for (const OnExitCallBack& cb : vecOnExitCallback) cb();

	SDL_Quit();

	return 0;
}