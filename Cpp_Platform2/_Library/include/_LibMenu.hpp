

#ifndef __LIB_VECTOR_HPP_INCLUDED_
#include <vector>
#include <string>
#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
class _LibMenuIndex{
public:
	u32 curr; // zero base
	u32 prev; // zero base
	u32 max; // if max = 3, curr should be only 0, 1, 2 !!!
	
	_LibMenuIndex(){}; // Do nothing.
	~_LibMenuIndex(void){}; // Do nothing.
	void Init(u32 in_curr, u32 in_max);
	void Increase(bool wrapAround = false);
	void Decrease(bool wrapAround = false);
	void Set(u32 index, bool fit_to_max = false);
	void Add(u32 index, bool wrapAround = false);
	void Minus(u32 index, bool wrapAround = false);
};

class _LibVectorMenu{
public:
	std::vector<std::string> strVector;
	std::vector<u32> userData;
	_LibMenuIndex menuIndex;
	u32 maxStrLen;
	
	_LibVectorMenu(){maxStrLen=0;};
	~_LibVectorMenu(void){}; // Do nothing.
	void Add(char *buf, u32 len, u32 userData = 0);
	void Clear(void);
	const char *GetPaddingString(u32 strVector_index, u32 min_len);
	void DoAllStringPadding(u32 min_len);
	void AddPrefixString(const char *format);
};

class _LibVectorData{
public:
	std::vector<u8 *> dataVector;
	std::vector<u32> dataLenVector;
	std::vector<u32> userData;
	
	_LibVectorData(){}; // Do nothing.
	~_LibVectorData(void){Clear();};
	void Add(u8 *buf, u32 len, u32 user_data);
	void Clear(void);
};

void _LibMenu_Demo(void);

// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================
// NULL on pure cpp library

#define __LIB_VECTOR_HPP_INCLUDED_
#endif//__LIB_VECTOR_HPP_INCLUDED_


