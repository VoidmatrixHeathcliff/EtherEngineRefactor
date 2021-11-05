#ifndef _MAIN_H_
#define _MAIN_H_

#include "Macros.h"
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
#include <cJSON.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <unordered_map>

#ifndef __WINDOWS__
extern char** environ;
#endif

#endif // !_MAIN_H_