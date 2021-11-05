#ifndef _MACROS_H_
#define _MACROS_H_

/* ���� _ETHER_DEUBG_ �Կ���Debugģʽ */
#ifndef _ETHER_DEBUG_
#define _ETHER_DEBUG_
#endif

static const char* VERSION_ENGINE = "Ver.202111.0.1(ISE)";

#define ETHER_API extern "C" int

struct Macro
{
	const char* name;
	int value;
};

#endif // !_MACROS_H_
