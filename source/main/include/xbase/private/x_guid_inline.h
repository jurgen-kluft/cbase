

//===============================================================================
// FUNCTIONS
//===============================================================================

//-------------------------------------------------------------------------------

inline 
xguid::xguid()
{
	setNull();
}

//-------------------------------------------------------------------------------

inline 
xguid::xguid(const xguid& other)
{
	for (s32 i=0; i<SIZE32; ++i)
		mGuid.ma32[i] = other.mGuid.ma32[i];
}

//-------------------------------------------------------------------------------

inline 
xguid::xguid(u32 wordh, u32 word2, u32 word1, u32 wordl)
{
	set(wordh, word2, word1, wordl);
}


//-------------------------------------------------------------------------------

inline
void xguid::setNull( void )                
{ 
	set(0,0,0,0);
}

//-------------------------------------------------------------------------------

inline
bool xguid::isNull( void ) const          
{ 
    return mGuid.ma32[0]==0 && mGuid.ma32[1]==0 && mGuid.ma32[2]==0 && mGuid.ma32[3]==0; 
}   

//-------------------------------------------------------------------------------

inline
void xguid::set(u32 wordh, u32 word2, u32 word1, u32 wordl)
{
	mGuid.ma32[0] = wordh;
	mGuid.ma32[1] = word2;
	mGuid.ma32[2] = word1;
	mGuid.ma32[3] = wordl;
}

//-------------------------------------------------------------------------------

inline
void xguid::get(u32& wordh, u32& word2, u32& word1, u32& wordl) const
{ 
    wordh = mGuid.ma32[0];
	word2 = mGuid.ma32[1];
	word1 = mGuid.ma32[2];
	wordl = mGuid.ma32[3];
}



