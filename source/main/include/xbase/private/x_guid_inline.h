

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
	for (s32 i=0; i<4; ++i)
		mGuid[i] = other.mGuid[i];
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
    return mGuid[0]==0 && mGuid[1]==0 && mGuid[2]==0 && mGuid[3]==0; 
}   

//-------------------------------------------------------------------------------

inline
void xguid::set(u32 wordh, u32 word2, u32 word1, u32 wordl)
{
	mGuid[0] = wordh;
	mGuid[1] = word2;
	mGuid[2] = word1;
	mGuid[3] = wordl;
}

//-------------------------------------------------------------------------------

inline
void xguid::get(u32& wordh, u32& word2, u32& word1, u32& wordl) const
{ 
    wordh = mGuid[0];
	word2 = mGuid[1];
	word1 = mGuid[2];
	wordl = mGuid[3];
}



