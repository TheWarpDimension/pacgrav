#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#define MERS_RAND_MAX 0xffffffff
unsigned long MersRand();

inline float frand(float range = 1.0f) { return range * ((float)MersRand() / (float)MERS_RAND_MAX); }
inline float sfrand(float range = 1.0f) { return (0.5f - (float)MersRand() / (float)MERS_RAND_MAX) * range; }

#ifndef M_PI
 #define M_PI 3.1415926535897932384626f
#endif

#ifndef max
 #define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
 #define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define sign(a)				((a) < 0 ? -1 : 1) 
#define signf(a)			((a) < 0.0f ? -1.0f : 1.0f) 


// Performs fast float to int conversion. It may round up or down, depending on
// the current CPU rounding mode - so DON'T USE IT if you want repeatable results
inline int FastFloatToInt(float a) 
{
	int retval;

	__asm
	{
		fld a
//		fist retval
		fistp retval
	}

	return retval;
}


inline int Round(float a)
{
	return floorf(a + 0.5f);
}


#define clamp(a, low, high)	if (a>high) a = high; else if (a<low) a = low;

#define NearlyEquals(a, b)	(fabsf((a) - (b)) < 1e-6 ? 1 : 0)

float Log2(float x);

#define ASSERT_FLOAT_IS_SANE(x) \
	DebugAssert((x + 1.0f) != x);

// Imagine that you have a stationary object that you want to move from
// A to B in _duration. This function helps you do that with linear acceleration
// to the midpoint, then linear deceleration to the end point. It returns the
// fractional distance along the route.
double RampUpAndDown(double _startTime, double _duration, double _timeNow);


#endif
