#include <lib/universal_include.h>

#include <ctype.h>
#include "lib/string_utils.h"


bool StringStartsWith(char const *theString, char const *prefix)
{
	while (*prefix != '\0')
	{
		if (*theString == '\0' || tolower(*theString) != tolower(*prefix)) return false;

		theString++;
		prefix++;
	}

	return true;
}


char *StringDuplicate(char const *theString)
{
	int len = strlen(theString) + 1;
	char *rv = new char[len];
	ReleaseAssert(rv, "Ran out of memory trying to duplicate a string");
	memcpy(rv, theString, len);
	return rv;
}


void StringToLower(char *theString)
{
	while (theString[0] != '\0')
	{
		theString[0] = tolower(theString[0]);
		theString++;
	}
}


// Creates a regular expression that will match the specified string.
// Basically, this function just escapes all the special magic RE
// characters such as * ^ $ etc
char *EscapeSpecialReChars(char const *searchString)
{
	int len = strlen(searchString);
	char *rv = new char [len*2 + 1];
	char *c = rv;
	while (*searchString)
	{
		if (strchr("^[]()?{$.*+|\\", *searchString))
		{
			*c = '\\';
			c++;
		}
		*c = *searchString;
		c++;
		searchString++;
	}

	*c = '\0';
	return rv;
}


void ConvertGlobIntoRegExp(char const *glob, char *reBuf, int reBufLen)
{
	int rePos = 0;
	reBufLen--;	// Make sure we always have space for the NULL terminator

	while (*glob)
	{
		if (*glob == '*')
		{
			reBuf[rePos] = '.';
			rePos++; if (rePos >= reBufLen) goto exit;
		}
		else if (*glob == '.')
		{
			reBuf[rePos] = '\\';
			rePos++; if (rePos >= reBufLen) goto exit;
		}

		reBuf[rePos] = *glob;
		rePos++; if (rePos >= reBufLen) goto exit;
		glob++;
	}

exit:
	reBuf[rePos] = '\0';
	reBuf[reBufLen] = '\0';
}


// Like itoa() but inserts commas every 3 digits
// The maximum size needed for buf is 15 characters, including the trailing null.
void MakeNiceNumberString(char *buf, int num)
{
	char *b = buf;
	char *c = buf;
	if (num < 0)
	{
		*c = '-';
		num = -num;
		b++;
		c++;
	}
	
	int i = 0;
	while (num)
	{
		if (i == 3 || i == 6 || i == 9) 
		{
			c[i] = ',';
			c++;
		}
		c[i] = '0' + num % 10;
		num /= 10;
		i++;
	}
	c[i] = '\0';

	c += i - 1;
	while (b < c)
	{
		char temp = *b;
		*b = *c;
		*c = temp;
		b++;
		c--;
	}
}


static bool IsWordChar(char c)
{
	if (isalnum(c) || c == '_') return true;
	return false;
}


bool IsWordBreak(char a, char const *l, int x)
{
	if (x < 0 || x >= strlen(l))
	{
		return true;
	}

	char b = l[x];

	if (isspace(b))
	{
		return !isspace(a);
	}
	else
	{
		return IsWordChar(a) != IsWordChar(b);
	}
	
//	return false;
}
