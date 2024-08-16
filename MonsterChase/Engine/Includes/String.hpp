#pragma once

#include "DynamicQueue.hpp"

class String {

	public:

						String			(const char * pChar, UInt64 pLen) noexcept;
						~String			() noexcept;		
		
		void			SetString		(const char * pChar, UInt64 pLen) noexcept;
		void	        SetString       (const String * pStr) noexcept;

		void			Append			(const char * pChar, UInt64 pLen) noexcept;
		void			Append			(const String * pStr) noexcept;

		void			Print			() const noexcept;

		const char *	GetString		() const noexcept; 

	private:

		friend class StringUtils;

		UInt64		vCharLen;		// length of the num of characters in the array (doesn't include null char).
		char *      vString;		// the character array that holds the string value.
};
