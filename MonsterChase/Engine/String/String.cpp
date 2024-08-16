
#include "String.hpp"

String::String (const char * pChar, UInt64 pLen) noexcept
{
	// TODO: debug assert if pChar is nullptr.

	vCharLen = pLen;
	vString  = (char *) MemMgr::MALLOC ((size_t) (((vCharLen + 1) * sizeof(char))));

	memcpy (vString, pChar, (size_t) (pLen * sizeof(char)));

	vString[vCharLen] = 0;	// set the last character to null.
}

String::~String	() noexcept
{
	free (vString);
	vString = nullptr;
}

void 
String::SetString (const char * pChar, UInt64 pLen) noexcept 
{
	// TODO: debug assert if pChar is nullptr.

	if (vCharLen != pLen) {

		// memory present is not the same as memory required. re-allocate.

		free (vString);

		vCharLen = pLen;
		vString = (char *) MemMgr::MALLOC ((size_t) ((vCharLen + 1) * sizeof(char)));
	}

	memcpy (vString, pChar, (size_t) (pLen * sizeof(char)));

	vString[vCharLen] = 0;	// set the last character to null.
}

void
String::SetString (const String * pStr) noexcept 
{
	SetString (pStr->vString, pStr->vCharLen);
}

void 
String::Append (const char * pChar, UInt64 pLen) noexcept 
{
	    char * temp;
		UInt64 newlen;

	// TODO: debug assert if pChar is nullptr.

	if (!pChar[0])
		return;	// input is an empty string. nothing to do.

	// re-allocate to make enough space for the string that needs to be appended.

	newlen = vCharLen + pLen;
	temp   = (char *) MemMgr::MALLOC ((size_t) ((newlen + 1)  * sizeof(char)));

	memcpy (temp, vString, (size_t) ((vCharLen + 1) * sizeof(char)));

	free (vString);

	vString = temp;

	memcpy (vString + vCharLen, pChar, (size_t) pLen);

	vCharLen = newlen;

	vString[vCharLen] = 0; // set the last character to null.
}

void 
String::Append (const String * pStr) noexcept 
{
	Append (pStr->vString, pStr->vCharLen);
}

void
String::Print () const noexcept 
{
	cout << vString;
}

const char *
String::GetString () const noexcept
{
	return vString;
}