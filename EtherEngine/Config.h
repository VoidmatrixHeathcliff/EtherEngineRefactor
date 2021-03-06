#ifndef _CONFIG_H_
#define _CONFIG_H_

#pragma warning(disable:26812)
#pragma warning(disable:26451)

#define ETHER_API extern "C"

static const char* VERSION_ENGINE			= "Ver.202111.0.1(ISE)";

static const char* ERRMSG_INVALIDENUM		= "invalid enum";
static const char* ERRMSG_INVALIDMEMBER		= "invalid member";

static const char* METANAME_HANDLEWINDOW	= "<SDL_Window>-2.0.16";
static const char* METANAME_HANDLERENDERER	= "<SDL_Renderer>-2.0.16";

static const char* METANAME_IMAGE			= "@Graphic.Image";
static const char* METANAME_TEXTURE			= "@Graphic.Texture";
static const char* METANAME_FONT			= "@Graphic.Font";

static const char* METANAME_MUSIC			= "@Media.Music";
static const char* METANAME_SOUND			= "@Media.Sound";

static const char* METANAME_EVENT			= "@Input.Event";

#endif // !_CONFIG_H_
