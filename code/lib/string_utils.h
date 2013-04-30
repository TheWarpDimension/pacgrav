#ifndef INCLUDED_STRING_UTILS_H
#define INCLUDED_STRING_UTILS_H


bool StringStartsWith(char const *theString, char const *prefix);
char *StringDuplicate(char const *theString);
void StringToLower(char *theString);
char *EscapeSpecialReChars(char const *searchString);
void ConvertGlobIntoRegExp(char const *glob, char *reBuf, int reBufLen);
void MakeNiceNumberString(char *buf, int num);
bool IsWordBreak(char a, char const *l, int x);

#endif
