/* event2/_project.h
 *
 * This file contains generated code. Do not modify!
 */

#if BUILDING_EVENT2 && defined _MSC_VER
#define EVENT2_EXPORT __declspec(dllexport)
#elif BUILDING_EVENT2
#define EVENT2_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define EVENT2_EXPORT __declspec(dllimport)
#else
#define EVENT2_EXPORT
#endif

