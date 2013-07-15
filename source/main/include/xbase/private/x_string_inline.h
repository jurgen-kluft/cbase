//==============================================================================
// PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!! PRIVATE!! 
//==============================================================================
//DOM-IGNORE-BEGIN

//------------------------------------------------------------------------------
// xstring_const_base 
//------------------------------------------------------------------------------

template<class T>
xstring_const_base<T>::xstring_const_base(const xstring& str)
:mBuffer(str.getLength(), str.c_str())
{
}

//------------------------------------------------------------------------------

template<class T>
xstring_const_base<T>::xstring_const_base(const xcstring& str)
:mBuffer(str.getLength(), str.c_str())
{
}

//------------------------------------------------------------------------------

template<class T>
xstring_const_base<T>::xstring_const_base(const xccstring& str)
:mBuffer(str.getLength(), str.c_str())
{
}

//------------------------------------------------------------------------------

template<class T>
inline s32 xstring_const_base<T>::getLength(void) const
{
	return mBuffer.getLength(); 
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::isEmpty(void) const
{
	return mBuffer.isEmpty(); 
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::isUpper(void) const
{
	return x_strIsUpper(c_str(), getLength());
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::isLower(void) const
{
	return x_strIsLower(c_str(), getLength());
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::isCapitalized(void) const
{
	return x_strIsCapitalized(c_str(), getLength());
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::isQuoted(void) const
{
	return isQuoted('\"');
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::isQuoted(char quote) const
{
	return x_strIsQuoted(c_str(), quote, getLength());
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::isDelimited(char left, char right) const
{
	return firstChar()==left && lastChar()==right;
}

//------------------------------------------------------------------------------

template<class T>
inline char xstring_const_base<T>::getAt(s32 inPosition) const
{
	ASSERT(inPosition>=0 && inPosition<getLength());
	const char* buffer = c_str();
	return buffer[inPosition];
}

//------------------------------------------------------------------------------

template<class T>
inline char xstring_const_base<T>::firstChar(void) const
{
	const char* buffer = c_str();
	return buffer[0];
}

//------------------------------------------------------------------------------

template<class T>
inline char xstring_const_base<T>::lastChar(void) const
{
	const char* buffer = c_str();
	return getLength()==0 ? '\0' : buffer[getLength()-1];
}


//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::startsWith(char inChar) const
{
	const char* buffer = c_str();
	return buffer[0] == inChar;
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::startsWith(const char* inStartStr) const
{
	return x_strStartsWith(c_str(), getLength(), inStartStr);
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::endsWith(char inChar) const
{
	return x_strEndsWith(c_str(), inChar, getLength());
}

//------------------------------------------------------------------------------

template<class T>
inline bool xstring_const_base<T>::endsWith(const char* inEndStr) const
{
	return x_strEndsWith(c_str(), getLength(), inEndStr);
}

///@name Comparison

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compare(const char* inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (x_strlen(inRHS)) : inCharNum;
	return x_strCompare(c_str(), getLength(), inRHS, inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compare(const xstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strCompare(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compare(const xstring_tmp& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strCompare(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compare(const xcstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strCompare(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compare(const xccstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strCompare(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compareNoCase(const char* inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (x_strlen(inRHS)) : inCharNum;
	return x_strCompareNoCase(c_str(), getLength(), inRHS, inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compareNoCase(const xstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strCompareNoCase(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compareNoCase(const xstring_tmp& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strCompareNoCase(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compareNoCase(const xcstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strCompareNoCase(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::compareNoCase(const xccstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strCompareNoCase(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqual(const char* inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (x_strlen(inRHS)) : inCharNum;
	return x_strEqual(c_str(), getLength(), inRHS, inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqual(const xstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strEqual(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqual(const xstring_tmp& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strEqual(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqual(const xcstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strEqual(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqual(const xccstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strEqual(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqualNoCase(const char* inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (x_strlen(inRHS)) : inCharNum;
	return x_strEqualNoCase(c_str(), getLength(), inRHS, inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqualNoCase(const xstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strEqualNoCase(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqualNoCase(const xstring_tmp& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strEqualNoCase(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqualNoCase(const xcstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strEqualNoCase(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::isEqualNoCase(const xccstring& inRHS, s32 inCharNum) const
{
	inCharNum = (inCharNum==-1) ? (inRHS.getLength()) : inCharNum;
	return x_strEqualNoCase(c_str(), getLength(), inRHS.c_str(), inCharNum);
}

///@name Search/replace

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::find(char inChar, s32 inPosition, s32 inCharNum) const
{
	ASSERT(inPosition >= 0);
	const s32 n = getLength();
	inCharNum = (inCharNum==-1) ? (n-inPosition) : inCharNum;
	ASSERT((inPosition+inCharNum) <= n);
	const char* pos = x_strFindInSubstr(c_str(), inChar, inPosition, inCharNum);
	if (pos==NULL)
		return -1;
	return (s32)pos - (s32)c_str();
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::find(const char* inString, s32 inPosition, s32 inCharNum) const
{
	ASSERT(inPosition >= 0);
	const s32 n = getLength();
	inCharNum = (inCharNum==-1) ? (n-inPosition) : inCharNum;
	ASSERT((inPosition+inCharNum) <= n);
	const char* pos = x_strFindInSubstr(c_str(), inString, inPosition, inCharNum);
	if (pos==NULL)
		return -1;
	return (s32)pos - (s32)c_str();
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::findNoCase(char inChar, s32 inPosition, s32 inCharNum) const
{
	ASSERT(inPosition >= 0);
	const s32 n = getLength();
	inCharNum = (inCharNum==-1) ? n : inCharNum;
	ASSERT((inPosition+inCharNum) <= n);
	const char* str = c_str();
	const char* find_pos = x_strFindNoCaseInSubstr(str, inChar, inPosition, inCharNum);
	return (find_pos) ? (find_pos - str) : -1;
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::findNoCase(const char* inString, s32 inPosition, s32 inCharNum) const
{
	ASSERT(inPosition >= 0);
	const s32 n = getLength();
	inCharNum = (inCharNum==-1) ? n : inCharNum;
	ASSERT((inPosition+inCharNum) <= n);
	const char* str = c_str();
	const char* find_pos = x_strFindNoCaseInSubstr(str, inString, inPosition, inCharNum);
	return (find_pos) ? (find_pos - str) : -1;
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::rfind(char inChar, s32 inPosition, s32 inLen) const
{
	inPosition = (inPosition==-1) ? (getLength()-1) : inPosition;
	inLen = (inLen==-1) ? (inPosition+1) : inLen;
	ASSERT(inPosition>=0 && inPosition<getLength());
	const char* pos = x_strRFind(c_str(), inChar, inPosition, inLen);
	return pos==NULL ? (-1) : (pos-c_str());
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::rfind(const char* inString, s32 inPosition, s32 inLen) const
{
	inPosition = (inPosition==-1) ? (getLength()-1) : inPosition;
	inLen = (inLen==-1) ? (inPosition+1) : inLen;
	ASSERT(inPosition>=0 && inPosition<getLength());
	const char* pos = x_strRFind(c_str(), inString, inPosition, inLen);
	return pos==NULL ? (-1) : (pos-c_str());
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::findOneOf(const char* inCharSet, s32 inPosition) const
{
	const char* pos = x_strFindOneOf(c_str(), inCharSet, inPosition, getLength());
	return pos==NULL ? (-1) : (pos-c_str());
}

//------------------------------------------------------------------------------

template<class T>
s32					xstring_const_base<T>::rfindOneOf(const char* inCharSet, s32 inPosition) const
{
	const char* pos = x_strRFindOneOf(c_str(), inCharSet, inPosition, getLength());
	return pos==NULL ? (-1) : (pos-c_str());
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::left(s32 inNum, xstring& outLeft) const
{
	XBOUNDS(inNum, 0, getLength());
	const char* buffer = c_str();
	outLeft.copy(buffer, x_intu::min(inNum, getLength()));
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::left(s32 inNum, xstring_tmp& outLeft) const
{
	XBOUNDS(inNum, 0, getLength());
	const char* buffer = c_str();
	outLeft.copy(buffer, x_intu::min(inNum, getLength()));
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::left(s32 inNum, xcstring& outLeft) const
{
	XBOUNDS(inNum, 0, getLength());
	const char* buffer = c_str();
	outLeft.copy(buffer, x_intu::min(inNum, getLength()));
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::right(s32 inNum, xstring& outRight) const
{
	const s32 l = getLength();
	XBOUNDS(inNum, 0, l);
	const char* buffer = c_str();
	inNum = x_intu::min(inNum, l);
	outRight.copy(buffer + l - inNum, inNum);
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::right(s32 inNum, xstring_tmp& outRight) const
{
	const s32 l = getLength();
	XBOUNDS(inNum, 0, l);
	const char* buffer = c_str();
	inNum = x_intu::min(inNum, l);
	outRight.copy(buffer + l - inNum, inNum);
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::right(s32 inNum, xcstring& outRight) const
{
	const s32 l = getLength();
	XBOUNDS(inNum, 0, l);
	const char* buffer = c_str();
	inNum = x_intu::min(inNum, l);
	outRight.copy(buffer + l - inNum, inNum);
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::mid(s32 inPosition, xstring& outMid, s32 inNum) const
{
	const s32 l = getLength();
	XBOUNDS(inPosition, 0, l);												// Must start to grab within string
	ASSERT((inNum==-1) || (inPosition+inNum<=l));			// Can't grab beyond end of string

	inPosition = x_intu::min(inPosition, l);
	inNum = (inNum==-1) ? (l-inPosition) : (x_intu::min(inNum, l-inPosition));
	const char* buffer = c_str();
	outMid.copy(buffer+inPosition, inNum);
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::mid(s32 inPosition, xstring_tmp& outMid, s32 inNum) const
{
	const s32 l = getLength();
	XBOUNDS(inPosition, 0, l);												// Must start to grab within string
	ASSERT((inNum==-1) || (inPosition+inNum<=l));			// Can't grab beyond end of string

	inPosition = x_intu::min(inPosition, l);
	inNum = (inNum==-1) ? (l-inPosition) : (x_intu::min(inNum, l-inPosition));
	const char* buffer = c_str();
	outMid.copy(buffer+inPosition, inNum);
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::mid(s32 inPosition, xcstring& outMid, s32 inNum) const
{
	const s32 l = getLength();
	XBOUNDS(inPosition, 0, l);												// Must start to grab within string
	ASSERT((inNum==-1) || (inPosition+inNum<=l));			// Can't grab beyond end of string

	inPosition = x_intu::min(inPosition, l);
	inNum = (inNum==-1) ? (l-inPosition) : (x_intu::min(inNum, l-inPosition));
	const char* buffer = c_str();
	outMid.copy(buffer+inPosition, inNum);
}

// xstring version

template<class T>
void				xstring_const_base<T>::substring(s32 inPosition, xstring& outSubstring, s32 inNum) const
{
	mid(inPosition, outSubstring, inNum);
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::substring(s32 inPosition, xstring& outSubstring) const
{
	mid(inPosition, outSubstring, getLength()-inPosition);
}


//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::splitOn(const char inChar, xstring& outLeft, xstring& outRight) const
{
	// Find the split character
	s32 split_pos = find(inChar);
	if (split_pos == -1) 
		return xFALSE;

	// Split string
	split(split_pos, true, outLeft, outRight);
	return xTRUE;
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::rsplitOn(const char inChar, xstring& outLeft, xstring& outRight) const
{
	// Find the split character
	int split_pos = rfind(inChar);
	if (split_pos == -1) 
		return xFALSE;

	// Split string
	split(split_pos, xTRUE, outLeft, outRight);
	return xTRUE;
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::split(s32 inPosition, bool inRemove, xstring& outLeft, xstring& outRight) const
{
	XBOUNDS(inPosition, 0, getLength());
	XBOUNDS(inPosition + ((int)inRemove), 0, getLength());

	left(inPosition, outLeft);
	mid(inPosition + ((int)inRemove), outRight);
}

// xstring_tmp version

template<class T>
void				xstring_const_base<T>::substring(s32 inPosition, xstring_tmp& outSubstring, s32 inNum) const
{
	mid(inPosition, outSubstring, inNum);
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::substring(s32 inPosition, xstring_tmp& outSubstring) const
{
	mid(inPosition, outSubstring, getLength()-inPosition);
}


//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::splitOn(const char inChar, xstring_tmp& outLeft, xstring_tmp& outRight) const
{
	// Find the split character
	s32 split_pos = find(inChar);
	if (split_pos == -1) 
		return xFALSE;

	// Split string
	split(split_pos, true, outLeft, outRight);
	return xTRUE;
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::rsplitOn(const char inChar, xstring_tmp& outLeft, xstring_tmp& outRight) const
{
	// Find the split character
	int split_pos = rfind(inChar);
	if (split_pos == -1) 
		return xFALSE;

	// Split string
	split(split_pos, xTRUE, outLeft, outRight);
	return xTRUE;
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::split(s32 inPosition, bool inRemove, xstring_tmp& outLeft, xstring_tmp& outRight) const
{
	XBOUNDS(inPosition, 0, getLength());
	XBOUNDS(inPosition + ((int)inRemove), 0, getLength());

	left(inPosition, outLeft);
	mid(inPosition + ((int)inRemove), outRight);
}

// xcstring version

template<class T>
void				xstring_const_base<T>::substring(s32 inPosition, xcstring& outSubstring, s32 inNum) const
{
	mid(inPosition, outSubstring, inNum);
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::substring(s32 inPosition, xcstring& outSubstring) const
{
	mid(inPosition, outSubstring, getLength()-inPosition);
}


//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::splitOn(const char inChar, xcstring& outLeft, xcstring& outRight) const
{
	// Find the split character
	s32 split_pos = find(inChar);
	if (split_pos == -1) 
		return xFALSE;

	// Split string
	split(split_pos, true, outLeft, outRight);
	return xTRUE;
}

//------------------------------------------------------------------------------

template<class T>
bool				xstring_const_base<T>::rsplitOn(const char inChar, xcstring& outLeft, xcstring& outRight) const
{
	// Find the split character
	int split_pos = rfind(inChar);
	if (split_pos == -1) 
		return xFALSE;

	// Split string
	split(split_pos, xTRUE, outLeft, outRight);
	return xTRUE;
}

//------------------------------------------------------------------------------

template<class T>
void				xstring_const_base<T>::split(s32 inPosition, bool inRemove, xcstring& outLeft, xcstring& outRight) const
{
	XBOUNDS(inPosition, 0, getLength());
	XBOUNDS(inPosition + ((int)inRemove), 0, getLength());

	left(inPosition, outLeft);
	mid(inPosition + ((int)inRemove), outRight);
}


//------------------------------------------------------------------------------
// xstring_mutable_base 
//------------------------------------------------------------------------------

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(void)
:__const_base()
{
}

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(s32 length)
:__const_base(length)
{
}

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(const char* str)
:__const_base(str)
{
}

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(s32 len, const char* str)
:__const_base(len, str)
{
}

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(s32 lenA, const char* strA, s32 lenB, const char* strB)
:__const_base(lenA, strA, lenB, strB)
{
}

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(const char* formatString, const x_va_list& args)
:__const_base(formatString,args)
{
}

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(const char* formatString, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
:__const_base(formatString,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
{
}

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(const xstring& str)
:__const_base(str.getLength(), str.c_str())
{
}

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(const xccstring& str)
:__const_base(str.getLength(), str.c_str())
{
}

template<class T>
xstring_mutable_base<T>::xstring_mutable_base(const xcstring& str)
:__const_base(str.getLength(), str.c_str())
{
}


// ---------------------------------------------------------------------------------------------------------------------------

template<class T>
void			xstring_mutable_base<T>::clear(void)
{
	__const_base::mBuffer.unbind();
	__const_base::mBuffer.setEmpty();
}

// ---------------------------------------------------------------------------------------------------------------------------

template<class T>
void			xstring_mutable_base<T>::repeat(const char* inString, s32 inTimes)
{
	repeat(inString, inTimes, -1);
}

// ---------------------------------------------------------------------------------------------------------------------------

template<class T>
void			xstring_mutable_base<T>::repeat(const char* inString, s32 inTimes, s32 inStringLength)
{
	ASSERT(inTimes >= 0);

	// Make sure string length is valid
	if (inStringLength < 0)
		inStringLength = x_strlen(inString);	// If not given, determine string length

	// Construct return string
	__const_base::mBuffer.resize(inStringLength * inTimes);	// Reserve length for string
	while (inTimes--)
		concat(inString, inStringLength);
}

template<class T>
void			xstring_mutable_base<T>::repeat	(const xstring& inString, s32 inTimes)
{
	repeat(inString.c_str(), inTimes, inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::repeat	(const xcstring& inString, s32 inTimes)
{
	repeat(inString.c_str(), inTimes, inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::repeat	(const xccstring& inString, s32 inTimes)
{
	repeat(inString.c_str(), inTimes, inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::repeat	(const xstring_tmp& inString, s32 inTimes)
{
	repeat(inString.c_str(), inTimes, inString.getLength());
}

template<class T>
s32				xstring_mutable_base<T>::format				(const char* formatString, const x_va_list& args)
{
	char* buffer = __const_base::mBuffer.getPointer();
	s32 len = x_vsprintf(buffer, __const_base::mBuffer.getMaxLength(), formatString, args);
	__const_base::mBuffer.setLength(len);
	__const_base::mBuffer.terminateWithZero();
	return len;
}

template<class T>
s32				xstring_mutable_base<T>::formatAdd			(const char* formatString, const x_va_list& args)
{
	char* buffer = __const_base::mBuffer.getPointer();
	s32 added = x_vsprintf(buffer + __const_base::mBuffer.getLength(), __const_base::mBuffer.getMaxLength(), formatString, args);
	__const_base::mBuffer.setLength(__const_base::mBuffer.getLength() + added);
	__const_base::mBuffer.terminateWithZero();
	return added;
}



template<class T>
void			xstring_mutable_base<T>::setAt	(s32 inPosition, char inChar)
{
	ASSERT(!__const_base::mBuffer.isEmpty());
	ASSERT(inPosition < __const_base::getLength());
	__const_base::mBuffer.unique();
	char* buffer = __const_base::mBuffer.getPointer();
	buffer[inPosition] = inChar;
}

template<class T>
void			xstring_mutable_base<T>::setAt	(s32 inPosition, const char* inString, s32 inNum)
{
	replace(inPosition, inString, inNum);
}

template<class T>
void			xstring_mutable_base<T>::setAt	(s32 inPosition, s32 inLen, const char* inString, s32 inNum)
{
	replace(inPosition, inLen, inString, inNum);
}

template<class T>
void			xstring_mutable_base<T>::replace(s32 inPosition, const char* inString, s32 inNum)
{
	replace(inPosition, 1, inString, inNum);
}

template<class T>
void			xstring_mutable_base<T>::replace(s32 inPosition, s32 inLength, const char* inString, s32 inNumChars)
{
	const s32 length =__const_base::getLength();
	inNumChars = (inNumChars==-1) ? (x_strlen(inString)) : (inNumChars);

	ASSERT(inLength >= 0);
	XBOUNDS(inPosition, 0, length - 1);
	XBOUNDS(inPosition + inLength, 0, length);
	ASSERT(inNumChars >= 0 && inNumChars <= x_strlen(inString));

	const s32 replaceEndPos = inPosition + inLength;
	const s32 dstEndPos     = inPosition + inNumChars;

	s32 extra = inNumChars - inLength;
	if (inNumChars <= inLength)
	{
		// Remove some data from the string by replacement
		__const_base::mBuffer.unique();
		
		char* replaceStartPtr = __const_base::mBuffer.getPointer() + inPosition;
		char* replaceEndPtr   = __const_base::mBuffer.getPointer() + replaceEndPos;
		
		if (inNumChars < inLength)
			x_memcpy(replaceEndPtr + extra, replaceEndPtr, length - replaceEndPos + 1);
		x_memcpy(replaceStartPtr, (const char*)inString, inNumChars);
	}
	else
	{
		if (__const_base::mBuffer.isGrowable())
		{
			// We can grow the buffer, so replacing is quite straightforward
			// Insert zero or more characters into the string
			__const_base::mBuffer.resize(length + extra);
			
			char* replaceStartPtr = __const_base::mBuffer.getPointer() + inPosition;
			char* replaceEndPtr   = __const_base::mBuffer.getPointer() + replaceEndPos;
			
			x_memmove(replaceEndPtr + extra, replaceEndPtr, length - replaceEndPos + 1);
			x_memcpy(replaceStartPtr, (const char*)inString, inNumChars);
		}
		else
		{
			// We cannot grow the buffer, so replacing is done by clamping against the maximum length
			const s32 maxLength = getMaxLength();

			char* replaceStartPtr = __const_base::mBuffer.getPointer() + inPosition;
			char* replaceEndPtr   = __const_base::mBuffer.getPointer() + replaceEndPos;

			s32 numCharsToMove = length - replaceEndPos + 1;
			if ((dstEndPos + numCharsToMove) > maxLength)
				numCharsToMove = maxLength - dstEndPos;
			if (numCharsToMove>0)
				x_memmove(replaceEndPtr + extra, replaceEndPtr, numCharsToMove);

			s32 numCharsToCopy = inNumChars;
			if ((inPosition+numCharsToCopy) > maxLength)
				numCharsToCopy = maxLength - inPosition;
			if (numCharsToCopy>0)
				x_memcpy(replaceStartPtr, (const char*)inString, numCharsToCopy);

			if (length+extra > maxLength)
				extra = maxLength - length;
		}
	}
	__const_base::mBuffer.setLength(length + extra);
	__const_base::mBuffer.terminateWithZero();
}

template<class T>
s32				xstring_mutable_base<T>::replace(const char* inFind, const char* inSubstitute, s32 inPosition)
{
	s32 num_replacements = 0;
	s32 str_pos = __const_base::find(inFind, inPosition);

	if (str_pos != -1)
	{
		s32 findLen = x_strlen(inFind);
		s32 substLen = x_strlen(inSubstitute);
		while (str_pos != -1)
		{
			replace(str_pos, findLen, inSubstitute);
			++num_replacements;
			str_pos = __const_base::find(inFind, str_pos+substLen);
		}
	}

	return num_replacements;
}

template<class T>
s32				xstring_mutable_base<T>::replace(char inFind, const char* inSubstitute, s32 inPosition)
{
	char find[3];
	find[0]=inFind;
	find[1]='\0';
	find[2]='\0';
	return replace(find, inSubstitute, inPosition);
}

template<class T>
s32				xstring_mutable_base<T>::replace(const char* inFind, char inSubstitute, s32 inPosition)
{
	char subst[3];
	subst[0]=inSubstitute;
	subst[1]='\0';
	subst[2]='\0';
	return replace(inFind, subst, inPosition);
}

template<class T>
s32				xstring_mutable_base<T>::replace(char inFind, char inSubstitute, s32 inPosition)
{
	char find[3];
	find[0]=inFind;
	find[1]='\0';
	find[2]='\0';
	return replace(find, inSubstitute, inPosition);
}

template<class T>
s32				xstring_mutable_base<T>::replaceAnyWith(const char* inAny, char inWith, s32 inFrom, s32 inNum)
{
	if (inWith=='\0')
		return 0;

	const s32 len = __const_base::getLength();
	if (inNum==-1)
		inNum = len;

	const s32 _To = (inFrom + inNum) <= len ? (inFrom + inNum) : len;

	char* buffer = __const_base::mBuffer.getPointer();

	s32 count = 0;
	while (*inAny != '\0')
	{
		char any = *inAny++;
		for (s32 i=inFrom; i<_To; ++i)
		{
			if (buffer[i] == any)
			{
				++count;
				buffer[i] = inWith;
			}
		}
	}
	return count;
}

template<class T>
void			xstring_mutable_base<T>::insert	(char inChar)
{
	insert(0, &inChar, 1);
}

template<class T>
void			xstring_mutable_base<T>::insert	(const char* inString, s32 inCharNum)
{
	insert(0, inString, inCharNum);
}

template<class T>
void			xstring_mutable_base<T>::insert	(const xstring& inString)
{
	insert(0, inString.c_str(), inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::insert	(const xcstring& inString)
{
	insert(0, inString.c_str(), inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::insert	(const xccstring& inString)
{
	insert(0, inString.c_str(), inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::insert	(const xstring_tmp& inString)
{
	insert(0, inString.c_str(), inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::insert	(s32 inPosition, char inChar)
{
	insert(inPosition, &inChar, 1);
}

template<class T>
void			xstring_mutable_base<T>::insert	(s32 inPosition, const char* inString, s32 inCharNum)
{
	if (inPosition == -1)
		inPosition = 0;

	const s32 l =__const_base::getLength();
	XBOUNDS(inPosition, 0, l);

	if (inCharNum == -1)
		inCharNum = x_strlen(inString);

	s32 addedLength = inCharNum;
	if (addedLength<=0)
		return;

	char* str = __const_base::mBuffer.getPointer();
	if (__const_base::mBuffer.isGrowable())
	{
		// Make sure our buffer is unique
		__const_base::mBuffer.resize(l + addedLength);
		str = __const_base::mBuffer.getPointer();

		// Move part of the string to make space to insert the string
		// We need to start at the end and also copy the zero terminator.
		char* pos  = str + l + addedLength;
		const char* read = str + l;
		const char* end  = str + inPosition;
		while (read>=end)
		{
			*pos = *read;
			--pos;
			--read;
		}

		// Copy the string into this string
		for (s32 i=0; i<addedLength; ++i)
			str[inPosition + i] = inString[i];
	}
	else
	{
		const s32 maxLength = getMaxLength();
		const s32 movingLength = l-inPosition;
		if (movingLength>0 && (inPosition+addedLength)<maxLength)
		{
			// Part of our string on the right side of inPosition will need to be moved
			x_memmove(str + inPosition + addedLength, str + inPosition, movingLength);
		}
		// How much of the incoming string can be copied into this string
		if ((inPosition + addedLength) >= maxLength)
			addedLength = maxLength - l;
		if (addedLength>0)
			x_memcpy(str + inPosition, inString, addedLength);

		// How much was actually added
		addedLength = ((l+addedLength)<maxLength) ? (addedLength) : (maxLength-l);
	}
	__const_base::mBuffer.setLength(l + addedLength);
	__const_base::mBuffer.terminateWithZero();
}

template<class T>
void			xstring_mutable_base<T>::insert	(s32 inPosition, const xstring& inString)
{
	insert(inPosition, inString.c_str(), inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::insert	(s32 inPosition, const xcstring& inString)
{
	insert(inPosition, inString.c_str(), inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::insert	(s32 inPosition, const xccstring& inString)
{
	insert(inPosition, inString.c_str(), inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::insert	(s32 inPosition, const xstring_tmp& inString)
{
	insert(inPosition, inString.c_str(), inString.getLength());
}

template<class T>
void			xstring_mutable_base<T>::remove	(s32 inStart, s32 inLength)
{
	XBOUNDS(inStart, 0,__const_base::getLength() - 1);
	XBOUNDS(inLength, 0,__const_base::getLength());
	XBOUNDS(inStart + inLength, 0,__const_base::getLength());

	if (inStart == 0 && inLength == __const_base::getLength())
	{
		__const_base::mBuffer.unbind();
		__const_base::mBuffer.setEmpty();
		return;
	}

	s32 l =__const_base::getLength();
	__const_base::mBuffer.unique();

	// Check if we're removing stuff at the end or in the middle of the string
	if (inStart + inLength != l)
		x_memcpy(__const_base::mBuffer.getPointer() + inStart, __const_base::mBuffer.getPointer() + inStart + inLength, l - (inStart + inLength));

	// Clean up
	__const_base::mBuffer.setLength(l-inLength);
	__const_base::mBuffer.terminateWithZero();
}

template<class T>
void			xstring_mutable_base<T>::remove	(const char* inCharSet)
{
	// Find how much initial chars we have to skip
	s32 strPos = __const_base::findOneOf(inCharSet);
	if (strPos!=-1)
	{
		__const_base::mBuffer.unique();
		xccstring charSetStr(inCharSet);
		while (strPos!=-1)
		{
			const s32 l =__const_base::getLength();
			s32 nextPos = strPos+1;
			while (nextPos<l && charSetStr.contains(__const_base::mBuffer.getPointer()[nextPos]))
				nextPos++;
			nextPos--;

			// Two cases
			// 1) We have more characters and last found character is at the end of the string
			// 2) We have more characters and next found character is not at the end of the string
			if (nextPos==(l-1))
			{
				// Remove range [strPos, EOS] characters
				__const_base::mBuffer.setLength(strPos);
				__const_base::mBuffer.terminateWithZero();
				break;
			}
			else
			{
				// Remove range [strPos, nextPos] characters, nextPos!=EOS
				const s32 afterNextPos = nextPos+1;
				x_memmove(__const_base::mBuffer.getPointer()+strPos, __const_base::mBuffer.getPointer()+afterNextPos, l-afterNextPos);
				__const_base::mBuffer.setLength(l - (afterNextPos-strPos));
				__const_base::mBuffer.terminateWithZero();
			}
			strPos = __const_base::findOneOf(inCharSet);
		}
	}
}

template<class T>
void			xstring_mutable_base<T>::upper(void)
{
	__const_base::mBuffer.unique();
	for (s32 i=0; i<__const_base::mBuffer.getLength(); ++i)
		__const_base::mBuffer.getPointer()[i] = x_toupper(__const_base::mBuffer.getPointer()[i]);
}

template<class T>
void			xstring_mutable_base<T>::lower(void)
{
	__const_base::mBuffer.unique();
	for (s32 i=0; i<__const_base::mBuffer.getLength(); ++i)
		__const_base::mBuffer.getPointer()[i] = (char)x_tolower(__const_base::mBuffer.getPointer()[i]);
}

template<class T>
void			xstring_mutable_base<T>::capitalize	(void)
{
	capitalize(NULL);
}

template<class T>
void			xstring_mutable_base<T>::capitalize	(const char* inSeperators)
{
	__const_base::mBuffer.unique();

	// Capitalize first letter of every word
	if (__const_base::mBuffer.getLength() > 0)
	{
		for (int i=0; i<__const_base::mBuffer.getLength(); ++i)
		{
			__const_base::mBuffer.getPointer()[i] = x_toupper(__const_base::mBuffer.getPointer()[i]);

			// Lower rest of word
			for (s32 j=i+1; j<__const_base::mBuffer.getLength(); ++j,++i)
			{
				if (inSeperators!=NULL && x_strFindOneOf(&(__const_base::mBuffer.getPointer()[j]), inSeperators, 0, 1)!=NULL)
				{
					i = j;
					break;
				}
				__const_base::mBuffer.getPointer()[j] = x_tolower(__const_base::mBuffer.getPointer()[j]);
			}
		}
	}
}

template<class T>
void			xstring_mutable_base<T>::trim(void)
{
	trimRight();
	trimLeft();
}

template<class T>
void			xstring_mutable_base<T>::trimLeft(void)
{
	// Find how much initial spaces we have to skip
	s32 skip=0;
	for (; skip < __const_base::getLength(); ++skip)
		if (!x_isspace(__const_base::getAt(skip)))
			break;

	// Move memory down in string and set new string length
	if (skip)
	{
		__const_base::mBuffer.unique();
		x_memmove(__const_base::mBuffer.getPointer(), __const_base::mBuffer.getPointer()+skip,__const_base::getLength()-skip+1);
		__const_base::mBuffer.setLength(__const_base::getLength()-skip);
	}
}

template<class T>
void			xstring_mutable_base<T>::trimRight(void)
{
	// Check how many spaces at the end can be cropped
	s32 new_len=__const_base::getLength();
	for (; new_len>0; new_len--)
		if (!x_isspace(__const_base::getAt(new_len-1)))
			break;

	// Set new length
	if (new_len !=__const_base::getLength())
	{
		__const_base::mBuffer.unique();
		__const_base::mBuffer.setLength(new_len);
		__const_base::mBuffer.terminateWithZero();
	}
}

template<class T>
void			xstring_mutable_base<T>::trim(char inChar)
{
	trimRight(inChar);
	trimLeft(inChar);
}

template<class T>
void			xstring_mutable_base<T>::trimLeft(char inChar)
{
	// Check how many spaces at the end can be cropped
	s32 skip=0;
	for (; skip<__const_base::getLength(); ++skip)
		if (__const_base::getAt(skip) != inChar)
			break;

	// Move memory down in string and set new string length
	if (skip)
	{
		__const_base::mBuffer.unique();
		x_memmove(__const_base::mBuffer.getPointer(), __const_base::mBuffer.getPointer()+skip,__const_base::getLength()-skip+1);
		__const_base::mBuffer.setLength(__const_base::getLength()-skip);
	}
}

template<class T>
void			xstring_mutable_base<T>::trimRight(char inChar)
{
	// Check how many spaces at the end can be cropped
	s32 new_len=__const_base::getLength();
	for (; new_len>0; new_len--)
		if (__const_base::getAt(new_len-1) != inChar)
			break;

	// Set new length
	if (new_len !=__const_base::getLength())
	{
		__const_base::mBuffer.unique();
		__const_base::mBuffer.setLength(new_len);
		__const_base::mBuffer.terminateWithZero();
	}
}

template<class T>
void			xstring_mutable_base<T>::trim(const char* inCharSet)
{
	trimRight(inCharSet);
	trimLeft(inCharSet);
}

template<class T>
void			xstring_mutable_base<T>::trimLeft(const char* inCharSet)
{
	// Check how many spaces at the end can be cropped
	s32 skip=0;
	for (; skip<__const_base::getLength(); ++skip)
		if (!x_strFind(inCharSet, __const_base::getAt(skip)))
			break;

	// Move memory down in string and set new string length
	if (skip)
	{
		__const_base::mBuffer.unique();
		x_memmove(__const_base::mBuffer.getPointer(), __const_base::mBuffer.getPointer()+skip,__const_base::getLength()-skip+1);
		__const_base::mBuffer.setLength(__const_base::getLength()-skip);
	}
}

template<class T>
void			xstring_mutable_base<T>::trimRight(const char* inCharSet)
{
	// Check how many spaces at the end can be cropped
	s32 new_len=__const_base::getLength();
	for (; new_len>0; new_len--)
		if (x_strFind(inCharSet, __const_base::getAt(new_len-1))==NULL)
			break;

	// Set new length
	if (new_len !=__const_base::getLength())
	{
		__const_base::mBuffer.unique();
		__const_base::mBuffer.setLength(new_len);
		__const_base::mBuffer.terminateWithZero();
	}
}

template<class T>
void			xstring_mutable_base<T>::trimQuotes	(void)
{
	trimQuotes('\"');
}

template<class T>
void			xstring_mutable_base<T>::trimQuotes(char inQuote)
{
	return trimDelimiters(inQuote, inQuote);
}

template<class T>
void			xstring_mutable_base<T>::trimDelimiters(char inLeft, char inRight)
{
	if (__const_base::isDelimited(inLeft, inRight))
	{
		__const_base::mBuffer.unique();
		x_memmove(__const_base::mBuffer.getPointer(), __const_base::mBuffer.getPointer()+1,__const_base::getLength()-1);
		__const_base::mBuffer.setLength(__const_base::getLength()-2);
		__const_base::mBuffer.terminateWithZero();
	}
}

template<class T>
void			xstring_mutable_base<T>::reverse(void)
{
	__const_base::mBuffer.unique();
	for (s32 i1 = 0, i2 =__const_base::getLength() - 1; i1 < i2; ++i1, --i2)
		x_swap(__const_base::mBuffer.getPointer()[i1], __const_base::mBuffer.getPointer()[i2]);
}

template<class T>
void			xstring_mutable_base<T>::reverse(s32 inPosition, s32 inCharNum)
{
	if (inCharNum == -1)
		inCharNum =__const_base::getLength() - inPosition;
	else if (inPosition+inCharNum >__const_base::getLength())
		inCharNum =__const_base::getLength() - inPosition;

	char* str = __const_base::mBuffer.getPointer();
	for (s32 i1 = inPosition, i2 = inPosition + inCharNum - 1; i1 < i2; ++i1, --i2)
		x_swap(str[i1], str[i2]);

}


template<class T>
bool			xstring_mutable_base<T>::splitOn(char inChar, xstring& outRight)
{
	// Find the split character
	s32 split_pos = __const_base::find(inChar);
	if (split_pos == -1) 
		return xFALSE;

	// Split string
	split(split_pos, xTRUE, outRight);
	return xTRUE;
}

template<class T>
bool			xstring_mutable_base<T>::splitOn(char inChar, xstring& outLeft, xstring& outRight) const
{
	return __const_base::splitOn(inChar, outLeft, outRight);
}

template<class T>
bool			xstring_mutable_base<T>::rsplitOn(char inChar, xstring& outRight)
{
	// Find the split character
	s32 split_pos = __const_base::rfind(inChar);
	if (split_pos == -1) 
		return xFALSE;

	// Split string
	split(split_pos, xTRUE, outRight);
	return xTRUE;
}

template<class T>
bool			xstring_mutable_base<T>::rsplitOn(char inChar, xstring& outLeft, xstring& outRight) const
{
	return __const_base::rsplitOn(inChar, outLeft, outRight);
}

template<class T>
void			xstring_mutable_base<T>::split(s32 inPosition, bool inRemove, xstring& outRight)
{
	XBOUNDS(inPosition, 0,__const_base::getLength());
	XBOUNDS(inPosition + ((s32)inRemove), 0,__const_base::getLength());
	ASSERT((s32)&outRight != (s32)this);

	__const_base::mBuffer.unique();
	__const_base::mid(inPosition + ((s32)inRemove), outRight);
	__const_base::mBuffer.setLength(inPosition);
	__const_base::mBuffer.terminateWithZero();
}

template<class T>
void			xstring_mutable_base<T>::split(s32 inPosition, bool inRemove, xstring& outLeft, xstring& outRight) const
{
	__const_base::split(inPosition, inRemove, outLeft, outRight);
}

template<class T>
void            xstring_mutable_base<T>::copy(const char* inString, s32 inLength)
{
	ASSERT(inLength >= 0);
	ASSERT(inString);
	if (inString==NULL || inLength<0)
		return;

	// Check if we're assigning ourselves an empty string, in that case just reuse the global empty string buffer
	if (inLength==0 || *inString=='\0')
	{
		// Reference the one and only empty string buffer
		__const_base::mBuffer.unbind();
		__const_base::mBuffer.setEmpty();
	}
	else
	{
		s32 length = inLength;
		if (__const_base::mBuffer.isGrowable())
		{
			// Copy <inString> to this string
			__const_base::mBuffer.resize(length);
		}
		else
		{
			if (length>getMaxLength())
				length = getMaxLength();
		}
		x_memcpy(__const_base::mBuffer.getPointer(), inString, length);
		__const_base::mBuffer.setLength(length);
		__const_base::mBuffer.terminateWithZero();
	}
}

template<class T>
void            xstring_mutable_base<T>::copy(const char* str)
{
	copy(str, x_strlen(str));
}

template<class T>
void            xstring_mutable_base<T>::copy(const xstring& str)
{
	copy(str.c_str(), str.getLength());
}

template<class T>
void            xstring_mutable_base<T>::copy(const xstring_tmp& str)
{
	copy(str.c_str(), str.getLength());
}

template<class T>
void            xstring_mutable_base<T>::copy(const xcstring& str)
{
	copy(str.c_str(), str.getLength());
}

template<class T>
void            xstring_mutable_base<T>::copy(const xccstring& str)
{
	copy(str.c_str(), str.getLength());
}

template<class T>
void			xstring_mutable_base<T>::indexToRowCol(s32 index, s32& row, s32& col) const
{
	ASSERT(index >= 0);

	const char* buffer = __const_base::c_str();
	if (buffer!=NULL && index <__const_base::getLength())
	{
		s32 scan = 0;
		row = 1;
		col = 1;

		while (scan < index)
		{
			if (buffer[scan] == '\n')
			{
				row++;
				col = 1;
			}
			else
			{
				col++;
			}
			scan++;
		}
	}
	else
	{
		row = -1;
		col = -1;
	}
}

template<class T>
void			xstring_mutable_base<T>::concat(const char* inSource)
{
	concat(inSource, x_strlen(inSource));
}

template<class T>
void			xstring_mutable_base<T>::concat(const char* inSource, s32 inLength)
{
	ASSERT(inSource);
	ASSERT(inLength >= 0 && inLength <= x_strlen(inSource));
	ASSERT(__const_base::getLength() >= 0);

	s32 length =__const_base::getLength();
	s32 addedLength = inLength;
	s32 newTotalLen = length+addedLength;
	if (__const_base::mBuffer.isGrowable())
	{
		__const_base::mBuffer.resize(newTotalLen);
	}
	else
	{
		if (newTotalLen >= getMaxLength())
			newTotalLen = getMaxLength() - length;
	}
	if (addedLength > 0)
	{
		x_memcpy(__const_base::mBuffer.getPointer() + length, inSource, addedLength * sizeof(char));
		__const_base::mBuffer.setLength(newTotalLen);
		__const_base::mBuffer.terminateWithZero();
	}
}


//------------------------------------------------------------------------------

inline xcstring::xcstring(void* buffer, s32 bufferSize)
{
	mBuffer = (xstring_buffer_char(buffer, bufferSize));
}

//------------------------------------------------------------------------------

inline xcstring::xcstring(void* buffer, s32 bufferSize, const char* str)
{
	mBuffer = (xstring_buffer_char(buffer, bufferSize, str));
}

//------------------------------------------------------------------------------

inline xcstring::xcstring(void* buffer, s32 bufferSize, const char* formatString, const x_va& v1, const x_va& v2, const x_va& v3, const x_va& v4, const x_va& v5, const x_va& v6, const x_va& v7, const x_va& v8, const x_va& v9, const x_va& v10)
{
	mBuffer = (xstring_buffer_char(buffer, bufferSize, formatString,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10));
}

//------------------------------------------------------------------------------

inline xcstring::xcstring(void* buffer, s32 bufferSize, const char* formatString, const x_va_list& args)
{
	mBuffer = (xstring_buffer_char(buffer, bufferSize, formatString,args));
}

//------------------------------------------------------------------------------
inline xcstring::xcstring(void* buffer, s32 bufferSize, const xstring& other)
{
	mBuffer = (xstring_buffer_char(buffer, bufferSize));
	mBuffer.copyFrom(other.c_str(), other.getLength());
}

//------------------------------------------------------------------------------
inline xcstring::xcstring(void* buffer, s32 bufferSize, const xstring_tmp& other)
{
	mBuffer = (xstring_buffer_char(buffer, bufferSize));
	mBuffer.copyFrom(other.c_str(), other.getLength());
}

//------------------------------------------------------------------------------
inline xcstring::xcstring(void* buffer, s32 bufferSize, const xcstring& other)
{
	mBuffer = (xstring_buffer_char(buffer, bufferSize));
	mBuffer.copyFrom(other.c_str(), other.getLength());
}

//------------------------------------------------------------------------------
inline xcstring::xcstring(void* buffer, s32 bufferSize, const xccstring& other)
{
	mBuffer = (xstring_buffer_char(buffer, bufferSize));
	mBuffer.copyFrom(other.c_str(), other.getLength());
}

//------------------------------------------------------------------------------

inline char				xcstring::operator[] (s32 inIndex) const
{
	if (inIndex>=0 && inIndex<getLength())
		return mBuffer.getPointer()[inIndex];
	return '\0';
}

//------------------------------------------------------------------------------

inline char&				xcstring::operator[]	(s32 inIndex)
{
	if (inIndex<0 || inIndex>getMaxLength())
		inIndex = getMaxLength();
	return mBuffer.getPointer()[inIndex];
}


//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator=(const xstring& inRHS)
{
	mBuffer.setLength(0);
	*this += inRHS;
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator=(const xstring_tmp& inRHS)
{
	mBuffer.setLength(0);
	*this += inRHS;
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator=(const xcstring& inRHS)
{
	mBuffer.setLength(0);
	*this += inRHS;
	return *this;
}


//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator=(const xccstring& inRHS)
{
	mBuffer.setLength(0);
	*this += inRHS;
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator=(char inRHS)
{
	mBuffer.copyFrom(&inRHS, 1);
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator=(const char* inRHS)
{
	mBuffer.copyFrom(inRHS, x_strlen(inRHS));
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator+=(char inRHS)
{
	concat(&inRHS, 1);
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator+=(const char* inRHS)
{
	concat(inRHS);
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator+=(const xstring& inRHS)
{
	concat(inRHS.c_str(), inRHS.getLength());
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator+=(const xstring_tmp& inRHS)
{
	concat(inRHS.c_str(), inRHS.getLength());
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator+=(const xcstring& inRHS)
{
	concat(inRHS.c_str(), inRHS.getLength());
	return *this;
}

//------------------------------------------------------------------------------

inline xcstring&		xcstring::operator+=(const xccstring& inRHS)
{
	concat(inRHS.c_str(), inRHS.getLength());
	return *this;
}

//------------------------------------------------------------------------------

inline char xccstring::operator[](s32 inIndex) const
{
	if (inIndex>= 0 && inIndex < getLength())
		return c_str()[inIndex];
	return '\0';
}

//------------------------------------------------------------------------------

inline xccstring& xccstring::operator = (const char* str)
{
	mBuffer.mString = str;
	mBuffer.mLength = x_strlen(str);
	return *this;
}


//==============================================================================
// Global xstring operators
//==============================================================================

// operator +

inline xstring_tmp		operator+ (const xstring&		inLHS, char					inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), &inRHS, 1); }
inline xstring_tmp		operator+ (const xstring&		inLHS, const char*			inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS, x_strlen(inRHS)); }
inline xstring_tmp		operator+ (const xstring&		inLHS, const xstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xstring&		inLHS, const xstring_tmp&	inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xstring&		inLHS, const xcstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xstring&		inLHS, const xccstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }

inline xstring_tmp		operator+ (const xstring_tmp&	inLHS, char					inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), &inRHS, 1); }
inline xstring_tmp		operator+ (const xstring_tmp&	inLHS, const char*			inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS, x_strlen(inRHS)); }
inline xstring_tmp		operator+ (const xstring_tmp&	inLHS, const xstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xstring_tmp&	inLHS, const xstring_tmp&	inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xstring_tmp&	inLHS, const xcstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xstring_tmp&	inLHS, const xccstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }

inline xstring_tmp		operator+ (const xcstring&		inLHS, char					inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), &inRHS, 1); }
inline xstring_tmp		operator+ (const xcstring&		inLHS, const char*			inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS, x_strlen(inRHS)); }
inline xstring_tmp		operator+ (const xcstring&		inLHS, const xstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xcstring&		inLHS, const xstring_tmp&	inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xcstring&		inLHS, const xcstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xcstring&		inLHS, const xccstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }

inline xstring_tmp		operator+ (const xccstring&		inLHS, char					inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), &inRHS, 1); }
inline xstring_tmp		operator+ (const xccstring&		inLHS, const char*			inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS, x_strlen(inRHS)); }
inline xstring_tmp		operator+ (const xccstring&		inLHS, const xstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xccstring&		inLHS, const xstring_tmp&	inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xccstring&		inLHS, const xcstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }
inline xstring_tmp		operator+ (const xccstring&		inLHS, const xccstring&		inRHS)	{ return xstring_tmp(inLHS.c_str(), inLHS.getLength(), inRHS.c_str(), inRHS.getLength()); }

// operator <

inline bool				operator< (const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xstring_tmp&	inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xstring_tmp&	inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xstring_tmp&	inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xstring_tmp&	inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xstring_tmp&	inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xcstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xccstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }
inline bool				operator< (const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == -1; }

// operator >

inline bool				operator> (const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xstring_tmp&	inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xstring_tmp&	inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xstring_tmp&	inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xstring_tmp&	inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xstring_tmp&	inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xcstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xccstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }
inline bool				operator> (const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 1; }

// operator <=

inline bool				operator<=(const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xstring_tmp&	inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xstring_tmp&	inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xstring_tmp&	inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xstring_tmp&	inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xstring_tmp&	inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xcstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xccstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }
inline bool				operator<=(const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) <= 0; }

// operator >=

inline bool				operator>=(const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xstring_tmp&	inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xstring_tmp&	inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xstring_tmp&	inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xstring_tmp&	inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xstring_tmp&	inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xcstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xccstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }
inline bool				operator>=(const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) >= 0; }

// operator ==

inline bool				operator==(const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xstring_tmp&	inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xstring_tmp&	inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xstring_tmp&	inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xstring_tmp&	inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xstring_tmp&	inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xcstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xccstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }
inline bool				operator==(const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) == 0; }

// operator !=

inline bool				operator!=(const xstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xstring_tmp&	inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xstring_tmp&	inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xstring_tmp&	inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xstring_tmp&	inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xstring_tmp&	inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xcstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xcstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xcstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xcstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xcstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xccstring&		inLHS, const char*			inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xccstring&		inLHS, const xstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xccstring&		inLHS, const xstring_tmp&	inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xccstring&		inLHS, const xcstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }
inline bool				operator!=(const xccstring&		inLHS, const xccstring&		inRHS)	{ return inLHS.compare(inRHS) != 0; }

//==============================================================================
// END
//==============================================================================
//DOM-IGNORE-END
