
#include "StringUtils.hpp"

UInt8
StringUtils::CountDigit (UInt64 pNum) noexcept
{
		UInt8 numdigit;

	if (!pNum)
		return 1;

	numdigit = 0;

	while (pNum) {
	
		pNum = pNum / 10;
		++numdigit;
	}

	return numdigit;
}


String *
StringUtils::UInt2Str (UInt64 pNum) noexcept
{
		UInt64   numdigit;
		UInt64   ndx;
		UInt8    rem;
		String * str;

	numdigit = CountDigit (pNum);
	
	str = (String *) MemMgr::MALLOC (sizeof (String));

	str->vString  = (char *) MemMgr::MALLOC ((size_t) ((numdigit + 1) * sizeof(char)));
	str->vCharLen = numdigit;

	if (!pNum)
		str->vString[0] = vNumAsciiArray[pNum];
	else {
	
		// start filling the digits as characters from the end.

		ndx  = numdigit - 1;

		while (pNum) {
		
			rem               = pNum % 10;
			pNum			  = pNum / 10;
			str->vString[ndx] = vNumAsciiArray[rem];
			--ndx;
		}
	}

	str->vString[str->vCharLen] = 0;  // set the null character.

	return str;
}

UInt8
StringUtils::UInt2Str (UInt64 pNum, char * pStr, UInt64 pLen) noexcept
{
	UInt8    numdigit;
	UInt8    rem;
	UInt8    ndx;

	numdigit = CountDigit (pNum);

	if (numdigit > pLen)
		return 0;

	if (!pNum)
		pStr[0] = vNumAsciiArray[pNum];
	else {

		// start filling the digits as characters from the end.

		ndx  = numdigit - 1;

		while (pNum) {

			rem       = pNum % 10;
			pNum	  = pNum / 10;
			pStr[ndx] = vNumAsciiArray[rem];
			--ndx;
		}
	}

	pStr[numdigit] = 0;  // set the null character.

	return numdigit;
}

UInt64
StringUtils::CountNumChar (const char * pStr) noexcept
{
		UInt64 numchar;

	numchar = 0;

	while (*pStr++)
		++numchar;

	return numchar;
}