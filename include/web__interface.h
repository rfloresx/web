/* web__project.h
 *
 * This file contains generated code. Do not modify!
 */

#ifdef BUILDING_CORTO_WEB
#include "web__type.h"
#include "web__api.h"
#include "web__load.h"
#else
#include "corto/web/web__type.h"
#include "corto/web/web__api.h"
#include "corto/web/web__load.h"
#endif

#if BUILDING_CORTO_WEB && defined _MSC_VER
#define CORTO_WEB_DLL_EXPORTED __declspec(dllexport)
#elif BUILDING_CORTO_WEB
#define CORTO_WEB_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define CORTO_WEB_EXPORT __declspec(dllimport)
#else
#define CORTO_WEB_EXPORT
#endif

