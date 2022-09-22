#ifndef __SMART_DEFINE_H__
#define __SMART_DEFINE_H__

#include <list>

typedef void* SMART_HANDLE;

typedef struct tagPoint3D
{
    float x;
    float y;
    float z;
} POINT3D, * LPPOINT3D;
typedef std::list<POINT3D>      POINT3D_LIST;

typedef struct tagRegion2D
{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
} REGION2D, * LPREGION2D;
typedef std::list<REGION2D>     REGION2D_LIST;

typedef struct tagTriangle3D
{
    POINT3D vertex[3];
    int     index[3];
} TRIANGLE3D, *LPTRIANGLE3D;
typedef std::list<TRIANGLE3D>   TRIANGLE3D_LIST;

const unsigned int VISIBLE_PATH_COUNT = 4;

#endif