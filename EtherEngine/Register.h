#ifndef _REGISTER_H_
#define _REGISTER_H_

#include "ModuleWindow.h"
#include "ModuleGraphic.h"
#include "ModuleMedia.h"
#include "ModuleInteractivity.h"
#include "ModuleTime.h"
#include "ModuleOS.h"
#include "ModuleAlgorithm.h"
#include "ModuleNetwork.h"
#include "ModuleString.h"
#include "ModuleJSON.h"
#include "ModuleCompress.h"
#include "ModuleXML.h"

#include <lua.hpp>

#include <vector>
#include <string>

struct BuiltinModuleRegisterInfo
{
	std::string name;
	lua_CFunction callback_preload;
};

template <typename T>
inline int CallbackPreloadBuiltinModule(lua_State* pLuaVM)
{
	T::Instance().PushMetaDataToGlobal(pLuaVM);
	T::Instance().PushMoudleDataToStack(pLuaVM);

	return 1;
}

static const std::vector<BuiltinModuleRegisterInfo> vecBuiltinModuleRegister = {
	{ "@Algorithm",		CallbackPreloadBuiltinModule<ModuleAlgorithm> },
	{ "@Graphic",		CallbackPreloadBuiltinModule<ModuleGraphic> },
	{ "@Interactivity", CallbackPreloadBuiltinModule<ModuleInteractivity> },
	{ "@JSON",			CallbackPreloadBuiltinModule<ModuleJSON> },
	{ "@Media",			CallbackPreloadBuiltinModule<ModuleMedia> },
	{ "@Network",		CallbackPreloadBuiltinModule<ModuleNetwork> },
	{ "@OS",			CallbackPreloadBuiltinModule<ModuleOS> },
	{ "@String",		CallbackPreloadBuiltinModule<ModuleString> },
	{ "@Time",			CallbackPreloadBuiltinModule<ModuleTime> },
	{ "@Window",		CallbackPreloadBuiltinModule<ModuleWindow> },
	{ "@Compress",		CallbackPreloadBuiltinModule<ModuleCompress> },
	{ "@XML",			CallbackPreloadBuiltinModule<ModuleXML> },
};

#endif // !_REGISTER_H_
