#ifndef ____COMMON_H____
#define ____COMMON_H____

#ifndef NOMINMAX

#endif  /* NOMINMAX */

#ifndef MAKEDWORD
#define MAKEDWORD(l,h)                  ( ((DWORD)(WORD)l) | (((DWORD)(WORD)h)<<16) )
#endif

#ifndef MAKE_DATE
#define MAKE_DATE(year, month, day)     MAKEDWORD(MAKEWORD((day), (month)), (year))
#define GET_YEAR(date)                  HIWORD(date)
#define GET_MONTH(date)                 HIBYTE(LOWORD(date))
#define GET_DAY(date)                   LOBYTE(date)
#endif

#ifndef MAKE_TIME
#define MAKE_TIME(hour, minute, second) MAKEDWORD(MAKEWORD((second), (minute)), (hour))
#define GET_HOUR(time)                  HIWORD(time)
#define GET_MINUTE(time)                HIBYTE(LOWORD(time))
#define GET_SECOND(time)                LOBYTE(time)
#endif

#ifndef MAKE_VERSION
#define MAKE_VERSION(chMain, chMinor, wBuildNum)    MAKEDWORD(wBuildNum, MAKEWORD(chMinor, chMain))
#endif

#ifndef MAKELONGLONG
#define MAKELONGLONG(l, h)              ( ((LONGLONG)(DWORD)(l)) | (((LONGLONG)(DWORD)(h))<<32) )
#endif

#ifndef LODWORD
#define LODWORD(ull)                    ( (DWORD)(ull) )
#endif

#ifndef HIDWORD
#define HIDWORD(ull)                    ( (DWORD)((LONGLONG)(ull))>>32 )
#endif

#ifndef DWORD_PTR
#define DWORD_PTR                       DWORD
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr)                if (ptr) { delete (ptr); (ptr) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(ptr)          if (ptr) { delete[](ptr); (ptr) = NULL; }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(ptr)                  if (ptr) { free(ptr); (ptr) = NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(ptr)               if (ptr) { ptr->Release(); ptr = NULL; }
#endif

#ifndef SAFE_DELETE_OBJECT
#define SAFE_DELETE_OBJECT(hObj)        if(hObj) { DeleteObject(hObj); (hObj) = NULL; }
#endif

#ifndef RECT_W
#define RECT_W(pRect)                   ((pRect)->right - (pRect)->left)
#endif

#ifndef RECT_H
#define RECT_H(pRect)                   ((pRect)->bottom - (pRect)->top)
#endif

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                ((int)(short)HIWORD(lp))
#endif

#ifndef MAX_BUFFER_SIZE
#define MAX_BUFFER_SIZE                 0x0800
#endif

#ifndef MAX_NAME_LENGTH
#define MAX_NAME_LENGTH                 0x20
#endif

#ifndef MAX_PSWD_LENGTH
#define MAX_PSWD_LENGTH                 0x0F
#endif

// Buffer size for MD5 value
#define MD5_VALUE_LEN                   16

static const float EPSILON = 1e-6f;
static const float MATH_PI = 3.1415926536f;
static const float MATH_2_PI = 6.2831853072f;
static const float MATH_PI_2 = 1.5707963268f;
static const float MATH_PI_3 = 1.0471975512f;
static const float MATH_PI_4 = 0.7853981634f;
static const float MATH_PI_5 = 0.6283185307f;
static const float MATH_PI_6 = 0.5235987756f;

#define D_2_R(d)                        (((float)(d)) *  0.0174532925f)
#define R_2_D(r)                        (((float)(r)) * 57.2957795129f)

typedef unsigned int uint;
typedef unsigned long ulong;

#endif