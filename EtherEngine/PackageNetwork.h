#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "Config.h"

#include "lua.hpp"
#include <SDL.h>

#define CPPHTTPLIB_ZLIB_SUPPORT
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
using namespace httplib;

#include <vector>
#include <mutex>
#include <unordered_map>


#define ERRCODE_SUCCESS							1354
#define ERRCODE_UNKNOWN							1355
#define ERRCODE_CONNECTION						1356
#define ERRCODE_BINDIPADDRESS					1357
#define ERRCODE_READ							1358
#define ERRCODE_WRITE							1359
#define ERRCODE_EXCEEDREDRICTCOUNT				1360
#define ERRCODE_CANCELED						1361
#define ERRCODE_SSLCONNECTION					1362
#define ERRCODE_SSLLOADINGCERTS					1363
#define ERRCODE_SSLSERVERVERIFY					1364
#define ERRCODE_UNSUPPORTEDMBC					1365
#define ERRCODE_COMPRESSION						1366

#define METANAME_CLIENT							"Network.Client"
#define METANAME_SERVER							"Network.Server"
#define METANAME_SERVER_REQ						"Network.Server.Request"
#define METANAME_SERVER_RES						"Network.Server.Response"

#define GetClientDataAt1stPos()					(Client*)(*(void**)luaL_checkudata(L, 1, METANAME_CLIENT))
#define GetServerDataAt1stPos()					(E_Server*)(*(void**)luaL_checkudata(L, 1, METANAME_SERVER))
#define GetServerReqDataAt1stPos()				(Request*)(*(void**)luaL_checkudata(L, 1, METANAME_SERVER_REQ))
#define GetServerResDataAt1stPos()				(Response*)(*(void**)luaL_checkudata(L, 1, METANAME_SERVER_RES))

#define CheckClientDataAt1stPos(client)			luaL_argcheck(L, client, 1, "get client data failed")
#define CheckServerDataAt1stPos(server)			luaL_argcheck(L, server && server->pServer, 1, "get server data failed")
#define CheckServerReqDataAt1stPos(req)			luaL_argcheck(L, req, 1, "get request data failed")
#define CheckServerResDataAt1stPos(res)			luaL_argcheck(L, res, 1, "get response data failed")

#define CheckHandlerFunctionAt3rdPos()			luaL_argcheck(L, lua_isfunction(L, 3) && lua_gettop(L) == 3,\
													3, "the last param callback handler must be function")

#define GenReqHandlerRefKey(id, route, type)	id + type + route
#define GenExpHandlerRefKey(id)					id + "_error_handler"
#define GetRouteAt2ndPosAndGenRefKey(server, route, refKey, type)\
	route = luaL_checkstring(L, 2);\
	if (route.empty() || route[0] != '/') route = "/" + route;\
	refKey = GenReqHandlerRefKey(server->id, route, type);\
	server->refKeys.push_back(refKey);

#define PushReqAndResToCallbackStack(req, res)\
	Request request = req;\
	Request** uppRequest = (Request**)lua_newuserdata(L, sizeof(Request*));\
	*uppRequest = &request;\
	luaL_getmetatable(L, METANAME_SERVER_REQ);\
	lua_setmetatable(L, -2);\
	Response** uppResponse = (Response**)lua_newuserdata(L, sizeof(Response*));\
	*uppResponse = &res;\
	luaL_getmetatable(L, METANAME_SERVER_RES);\
	lua_setmetatable(L, -2);

struct RequestParam
{
	std::string route;
	Headers headers;
	std::string str_params;
	Params tab_params;
	std::string content_type;
};

struct E_Server
{
	Server* pServer;
	std::string id;
	std::vector<std::string> refKeys;
	E_Server(Server* pServer, const std::string& id)
		: pServer(pServer), id(id) {}
};

/// <summary>
/// ??Lua????????????????????????????????????????????
/// </summary>
/// <param name="L">Lua??????????</param>
/// <param name="reqParam">????????????????</param>
/// <returns>????????</returns>
const char* GetRequestParamAt2ndPos(lua_State* L, RequestParam& reqParam);

/// <summary>
/// ?? ErrorCode ????????
/// </summary>
/// <param name="error">ErrorCode</param>
/// <returns>??</returns>
inline int ConvertErrorCodeToMacro(const Error& error);

/// <summary>
/// ?? response ???? Lua ????
/// </summary>
/// <param name="error">Error</param>
/// <returns>??</returns>
void PushResponseToStack(lua_State* L, const Result& res);

/*
* ??????????????????????
* 1??????????????????userdata-Client??
* 1??????????????????????boolean??
*/
ETHER_API int client_CheckValid(lua_State* L);

/*
* ???? Get ??????????????
* 2??????????????????userdata-Client??????????????table????????????
*	{
*		route??string??????????
*		headers??table????????????????????????????????????????
*	}
* 1??????????????????table????????????
*	{
*		error??number??????????????
*		status??number???????????????????? nil??
*		body??string?????????????????????? nil??
*		headers??table???????????????????? nil
*	}
*/
ETHER_API int client_Get(lua_State* L);

/*
* ???? Post ??????????????
* 2??????????????????userdata-Client??????????????table????????????
*	{
*		route??string??????????
*		headers??table??????????????????????????????????????????
*		params??string/table????????????????????????????????
*		type??string?????????? MIME Type?????????????? ??application/x-www-form-urlencoded????
*	}
* 1??????????????????table????????????
*	{
*		error??number??????????????
*		status??number???????????????????? nil??
*		body??string?????????????????????? nil??
*		headers??table???????????????????? nil
*	}
*/
ETHER_API int client_Post(lua_State* L);

/*
* ???? Put ??????????????
* 2??????????????????userdata-Client??????????????table????????????
*	{
*		route??string??????????
*		headers??table??????????????????????????????????????????
*		params??string/table????????????????????????????????
*		type??string?????????? MIME Type?????????????? ??application/x-www-form-urlencoded????
*	}
* 1??????????????????table????????????
*	{
*		error??number??????????????
*		status??number???????????????????? nil??
*		body??string?????????????????????? nil??
*		headers??table???????????????????? nil
*	}
*/
ETHER_API int client_Put(lua_State* L);

/*
* ???? Patch ??????????????
* 2??????????????????userdata-Client??????????????table????????????
*	{
*		route??string??????????
*		headers??table??????????????????????????????????????????
*		params??string????????????????????????????????
*		type??string?????????? MIME Type?????????????? ??application/x-www-form-urlencoded????
*	}
* 1??????????????????table????????????
*	{
*		error??number??????????????
*		status??number???????????????????? nil??
*		body??string?????????????????????? nil??
*		headers??table???????????????????? nil
*	}
*/
ETHER_API int client_Patch(lua_State* L);

/*
* ???? Delete ??????????????
* 2??????????????????userdata-Client??????????????table????????????
*	{
*		route??string??????????
*		headers??table??????????????????????????????????????????
*		params??string????????????????????????????????
*		type??string?????????? MIME Type?????????????? ??application/x-www-form-urlencoded????
*	}
* 1??????????????????table????????????
*	{
*		error??number??????????????
*		status??number???????????????????? nil??
*		body??string?????????????????????? nil??
*		headers??table???????????????????? nil
*	}
*/
ETHER_API int client_Delete(lua_State* L);

/*
* ???? Options ??????????????
* 2??????????????????userdata-Client??????????????table????????????
*	{
*		route??string??????????
*		headers??table????????????????????????????????????????
*	}
* 1??????????????????table????????????
*	{
*		error??number??????????????
*		status??number???????????????????? nil??
*		body??string?????????????????????? nil??
*		headers??table???????????????????? nil
*	}
*/
ETHER_API int client_Options(lua_State* L);

/*
* ??????????????????????
* 2??????????????????userdata-Client????????????table??????????????????
* 0??????
*/
ETHER_API int client_SetDefaultHeaders(lua_State* L);

/*
* ????????????????????????
* 2??????????????????userdata-Client??????????????number??????????????
* 0??????
*/
ETHER_API int client_SetConnectTimeout(lua_State* L);

/*
* ????????????????????????????
* 2??????????????????userdata-Client??????????????number????????????
* 0??????
*/
ETHER_API int client_SetReadTimeout(lua_State* L);

/*
* ????????????????????????????
* 2??????????????????userdata-Client??????????????number????????????
* 0??????
*/
ETHER_API int client_SetWriteTimeout(lua_State* L);

/*
* ??????????????????????
* 2??????????????????userdata-Client??????????????????boolean??
* 0??????
*/
ETHER_API int client_SetKeepAlive(lua_State* L);

/*
* ????????????????????????
* 2??????????????????userdata-Client????????????????????boolean??
* 0??????
*/
ETHER_API int client_SetFollowRedirect(lua_State* L);

/*
* ??????????????????????????
* 2??????????????????userdata-Client??????????????boolean??
* 0??????
*/
ETHER_API int client_SetCompressRequest(lua_State* L);

/*
* ??????????????????????????
* 2??????????????????userdata-Client??????????????boolean??
* 0??????
*/
ETHER_API int client_SetCompressResponse(lua_State* L);

/*
* ?????????? CACert ?? SSL ????????????
* 2??????????????????userdata-Client??????????????????string??
* 0??????
*/
ETHER_API int client_SetCACertPath(lua_State* L);

/*
* ????????????????????
* 3??????????????????userdata-Client??????????????????string??????????????????number??
* 0??????
*/
ETHER_API int client_SetProxy(lua_State* L);

/*
* ??????????
* 1????????????????string??
* 1????????????????????userdata-Client??
*/
ETHER_API int createClient(lua_State* L);

/*
* ????????????GC????
* 1??????????????????userdata-Client??
* 0??????
*/
ETHER_API int __gc_Client(lua_State* L);

/// <summary>
/// ??????????????????????
/// </summary>
/// <param name="req">????????????</param>
/// <param name="res">????????????</param>
/// <param name="L">Lua ??????????</param>
/// <param name="refKey">??????????????????????????????</param>
/// <param name="serverID">??????ID</param>
void CallRequestHandler(const Request& req, Response& res, lua_State* L, const std::string& refKey, const std::string& serverID);

/// <summary>
/// ??????????????????????????
/// </summary>
/// <param name="req">????????????</param>
/// <param name="res">????????????</param>
/// <param name="L">Lua ??????????</param>
/// <param name="refKey">??????????????????????????????</param>
/// <param name="errmsg">????????</param>
void CallExceptionHandler(const Request& req, Response& res, lua_State* L, const std::string& refKey, const std::string& errmsg);

/*
* ????????????
* 1????????????????????????userdata-Request??
* 1??????????????????string??
*/
ETHER_API int request_GetMethod(lua_State* L);

/*
* ????????????
* 1????????????????????????userdata-Request??
* 1??????????????????string??
*/
ETHER_API int request_GetRoute(lua_State* L);

/*
* ??????????
* 1????????????????????????userdata-Request??
* 1????????????????table??
* ??????????Lua??table??????????????????????????????????????????????????
*	???????????????????????????????????????????????????? GetHeaderValue ????????????????????????????
*/
ETHER_API int request_GetHeaders(lua_State* L);

/*
* ??????????
* 1????????????????????????userdata-Request??
* 1????????????????string??
*/
ETHER_API int request_GetBody(lua_State* L);

/*
* ????????????????
* 1????????????????????????userdata-Request??
* 1??????????????????????string??
*/
ETHER_API int request_GetRemoteAddress(lua_State* L);

/*
* ??????????????????
* 1????????????????????????userdata-Request??
* 1????????????????????????number??
*/
ETHER_API int request_GetRemotePort(lua_State* L);

/*
* ??????????HTTP????
* 1????????????????????????userdata-Request??
* 1????????HTTP??????string??
*/
ETHER_API int request_GetVersion(lua_State* L);

/*
* ????????????
* 1????????????????????????userdata-Request??
* 1??????????????????table??
* ??????????Lua??table????????????????????????????????????????????????????
*	???????????????????????????????????????????????????? GetParamValue ????????????????????????????
*/
ETHER_API int request_GetParams(lua_State* L);

/*
* ??????????????????????????
* 2????????????????????????userdata-Request????????string??
* 1??????????????????boolean??
*/
ETHER_API int request_CheckHeaderKeyExist(lua_State* L);

/*
* ??????????????????????????
* 2??3????????????????????????userdata-Request????????string??????????number??????????????1??
* 1????????????string??
*/
ETHER_API int request_GetHeaderValue(lua_State* L);

/*
* ????????????????????????????????
* 2????????????????????????userdata-Request????????string??
* 1??????????????????number??
*/
ETHER_API int request_GetHeaderValueCount(lua_State* L);

/*
* ????????????????????????????
* 2????????????????????????userdata-Request????????string??
* 1??????????????????boolean??
*/
ETHER_API int request_CheckParamKeyExist(lua_State* L);

/*
* ????????????????????????????
* 2??3????????????????????????userdata-Request????????string??????????number??????????????1??
* 1????????????string??
*/
ETHER_API int request_GetParamValue(lua_State* L);

/*
* ??????????????????????????????????
* 2????????????????????????userdata-Request????????string??
* 1??????????????????number??
*/
ETHER_API int request_GetParamValueCount(lua_State* L);

/*
* ??????????HTTP????
* 2????????????????????????userdata-Response????HTTP??????string??
* 0??????
*/
ETHER_API int response_SetVersion(lua_State* L);

/*
* ????????????????
* 2????????????????????????userdata-Response????????????number??
* 0??????
*/
ETHER_API int response_SetStatus(lua_State* L);

/*
* ??????????
* 1????????????????????????userdata-Response??
* 1????????????????table??
* ??????????Lua??table??????????????????????????????????????????????????
*	???????????????????????????????????????????????????? GetHeaderValue ????????????????????????????
*/
ETHER_API int response_GetHeaders(lua_State* L);

/*
* ??????????
* 2????????????????????????userdata-Response????????????string??
* 0??????
*/
ETHER_API int response_SetBody(lua_State* L);

/*
* ??????????????????????????
* 2????????????????????????userdata-Response????????string??
* 1??????????????????boolean??
*/
ETHER_API int response_CheckHeaderKeyExist(lua_State* L);

/*
* ??????????????????????????
* 2??3????????????????????????userdata-Response????????string??????????number??????????????1??
* 1????????????string??
*/
ETHER_API int response_GetHeaderValue(lua_State* L);

/*
* ????????????????????????????????
* 2????????????????????????userdata-Response????????string??
* 1??????????????????number??
*/
ETHER_API int response_GetHeaderValueCount(lua_State* L);

/*
* ??????????????????????????
* 3????????????????????????userdata-Response????????string????????string??
* 0??????
*/
ETHER_API int response_SetHeaderValue(lua_State* L);

/*
* ??????????
* 3????????????????????????userdata-Response????????????table??
* 0??????
* ??????????Lua??table??????????????????????????????????????????????????
*	???????????????????????????????????????? SetHeaderValue ????????????????????????
*/
ETHER_API int response_SetHeaders(lua_State* L);

/*
* ????????????????????
* 2????????????????????????userdata-Response????????????????string??
* 0??????
*/
ETHER_API int response_SetRedirect(lua_State* L);

/*
* ??????????????
* 2??3????????????????????????userdata-Response????????????????string??????????????MIME??????string?????????????? text/plain??
* 0??????
*/
ETHER_API int response_SetContent(lua_State* L);

/*
* ??????????????????????
* 1??????????????????userdata-Server??
* 1??????????????????????boolean??
*/
ETHER_API int server_CheckValid(lua_State* L);

/*
* ??????????????????????
* 1??????????????????userdata-Server??
* 1??????????????????????boolean??
*/
ETHER_API int server_CheckRunning(lua_State* L);

/*
* ????????????Get????????????????????????
* 3??????????????????userdata-Server??????????string??????????????function??
* 0??????
* ??????????????????????????????????????2??????????????????????????userdata-Request??????????????????????userdata-Response??
*/
ETHER_API int server_Get(lua_State* L);

/*
* ????????????Post????????????????????????
* 3??????????????????userdata-Server??????????string??????????????function??
* 0??????
* ??????????????????????????????????????2??????????????????????????userdata-Request??????????????????????userdata-Response??
*/
ETHER_API int server_Post(lua_State* L);

/*
* ????????????Put????????????????????????
* 3??????????????????userdata-Server??????????string??????????????function??
* 0??????
* ??????????????????????????????????????2??????????????????????????userdata-Request??????????????????????userdata-Response??
*/
ETHER_API int server_Put(lua_State* L);

/*
* ????????????Patch????????????????????????
* 3??????????????????userdata-Server??????????string??????????????function??
* 0??????
* ??????????????????????????????????????2??????????????????????????userdata-Request??????????????????????userdata-Response??
*/
ETHER_API int server_Patch(lua_State* L);

/*
* ????????????Delete????????????????????????
* 3??????????????????userdata-Server??????????string??????????????function??
* 0??????
* ??????????????????????????????????????2??????????????????????????userdata-Request??????????????????????userdata-Response??
*/
ETHER_API int server_Delete(lua_State* L);

/*
* ????????????Options????????????????????????
* 3??????????????????userdata-Server??????????string??????????????function??
* 0??????
* ??????????????????????????????????????2??????????????????????????userdata-Request??????????????????????userdata-Response??
*/
ETHER_API int server_Options(lua_State* L);

/*
* ??????????????????????????
* 3??????????????????userdata-Server????????????string??????????????string??
* 0??????
*/
ETHER_API int server_SetMountPoint(lua_State* L);

/*
* ??????????????????????????????
* 2??????????????????userdata-Server????????????string??
* 0??????
*/
ETHER_API int server_RemoveMountPoint(lua_State* L);

/*
* ??????????????????????????????????????MIME????????
* 2??????????????????userdata-Server????????????????????????string????MIME??????string??
* 0??????
*/
ETHER_API int server_SetFileExtMapToMIMEType(lua_State* L);

/*
* ????????????????????????
* 2??????????????????userdata-Server??????????????????????function??
* 0??????
* ??????????????????3??????????????????????????userdata-Request??????????????????????userdata-Response??????????????string??
*/
ETHER_API int server_SetExceptionHandler(lua_State* L);

/*
* ??????????????????????????
* 2??????????????????userdata-Server????????????????????number????????5??
* 0??????
*/
ETHER_API int server_SetMaxKeepAliveCount(lua_State* L);

/*
* ????????????????????????????
* 2??????????????????userdata-Server??????????????????????number??????????????????5??
* 0??????
*/
ETHER_API int server_SetKeepAliveTimeout(lua_State* L);

/*
* ????????????????????????????
* 2??????????????????userdata-Server??????????????number????????????
* 0??????
*/
ETHER_API int server_SetReadTimeout(lua_State* L);

/*
* ????????????????????????????
* 2??????????????????userdata-Server??????????????number????????????
* 0??????
*/
ETHER_API int server_SetWriteTimeout(lua_State* L);

/*
* ????????????????????????????
* 2??????????????????userdata-Server??????????????number??????????????
* 0??????
*/
ETHER_API int server_SetIdleInterval(lua_State* L);

/*
* ????????????????????????????????????
* 2??????????????????userdata-Server??????????????number??????????????
* 0??????
*/
ETHER_API int server_SetMaxRequestLength(lua_State* L);

/*
* ??????????????????????????????????
* 1??????????????????userdata-Server??
* 1????????????????????????number??
*/
ETHER_API int server_BindToAnyPort(lua_State* L);

/*
* ????????????????????????????
* 1??????????????????userdata-Server??
* 0??????
* ????????????????????????Lua??????????
*/
ETHER_API int server_ListenAfterBind(lua_State* L);

/*
* ??????????????
* 3??????????????????userdata-Server??????????????string????????????number??
* 0??????
* ????????????????????????Lua??????????
*/
ETHER_API int server_Listen(lua_State* L);

/*
* ??????????????
* 1??????????????????userdata-Server??
* 0??????
*/
ETHER_API int server_Stop(lua_State* L);

/*
* ??????????
* 0??2??????SSL??????????????????????string??????????SSL??????????????????????string????????
* 1????????????????????userdata-Client??
* ???????????????????????????????????????????????????????????? HTTPS ????
*/
ETHER_API int createServer(lua_State* L);

/*
* ????????????GC????
* 1??????????????????userdata-Server??
* 0??????
*/
ETHER_API int __gc_Server(lua_State* L);

/*
* ?? HTTP/HTTPS ????????????????????????????????
* 1??????HTTP/HTTPS ??????string??
* 3??????????????????string??????????????????????????string???????? ??/????????????string??????????????????
*/
ETHER_API int splitLink(lua_State* L);

#endif // !_NETWORK_H_

