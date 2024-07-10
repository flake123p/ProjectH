
#include "Everything_Lib_Mgr.hpp"

#include <iostream>

using namespace std;

void _LibMenuIndex::Init(u32 in_curr, u32 in_max)
{
	curr = in_curr;
	prev = 0;
	max = in_max;
}

void _LibMenuIndex::Increase(bool wrapAround /* = false */)
{
	prev = curr;
	curr++;
	BASIC_ASSERT(curr <= max);
	if (curr == max) {
		if (wrapAround) {
			curr = 0;
		} else {
			curr = max - 1;
		}
	}
}

void _LibMenuIndex::Decrease(bool wrapAround /* = false */)
{
	prev = curr;
	if (curr == 0) {
		if (wrapAround) {
			curr = max-1;
		}
	} else {
		curr--;
	}
}

void _LibMenuIndex::Set(u32 index, bool fit_to_max /* = false */)
{
	prev = curr;
	if (index >= max) {
		if (fit_to_max) {
			curr = max-1;
		} else {
			curr = 0;
		}
	} else {
		curr = index;
	}
}

void _LibMenuIndex::Add(u32 index, bool wrapAround /* = false */)
{
	prev = curr;

	if (index + curr >= max) {
		if (wrapAround) {
			curr = (curr + index) % max;
		} else {
			curr = max-1;
		}
	} else {
		curr += index;
	}
}

void _LibMenuIndex::Minus(u32 index, bool wrapAround /* = false */)
{
	prev = curr;

	if (index <= curr) {
		curr = curr - index;
	} else {
		if (wrapAround) {
			index = index % max;
			index = max - index;
			Add(index, true);
		} else {
			curr = 0;
		}
	}
}
	
void _LibVectorMenu::Add(char *buf, u32 len, u32 userData /* = 0 */)
{
	string s0 (buf, 0, len);

	strVector.push_back(s0);
	this->userData.push_back(userData);
	
	menuIndex.Init(0, strVector.size());

	if (s0.length() > maxStrLen) {
		maxStrLen = s0.length();
	}
}

void _LibVectorMenu::Clear(void)
{
	strVector.clear();
	userData.clear();

	menuIndex.Init(0, strVector.size());

	maxStrLen = 0;
}

const char *_LibVectorMenu::GetPaddingString(u32 strVector_index, u32 min_len)
{
	static string s0;
	u32 curStrLen = strVector[strVector_index].length();
	if (curStrLen >= min_len) {
		return strVector[strVector_index].c_str();
	} else {
		s0 = strVector[strVector_index];
		s0.append(min_len-curStrLen, ' ');
		return s0.c_str();
	}
}

void _LibVectorMenu::DoAllStringPadding(u32 min_len)
{
	u32 curStrLen;

	for (u32 i=0; i<menuIndex.max; i++) {
		curStrLen = strVector[i].length();

		if (curStrLen < min_len) {
			strVector[i].append(min_len-curStrLen, ' ');
		}
	}
}

void _LibVectorMenu::AddPrefixString(const char *format)
{
	char tempStr[20];
	
	for (u32 i=0; i<menuIndex.max; i++) {
		sprintf(tempStr, format, i);
		strVector[i].insert(0, tempStr);
		
		if (strVector[i].length() > maxStrLen) {
			maxStrLen = strVector[i].length();
		}
		
	}
}

void _LibVectorData::Add(u8 *buf, u32 len, u32 user_data)
{
	u8 *ptr = (u8 *)malloc(len);
	BASIC_ASSERT(ptr != NULL);

	memcpy(ptr, buf, len);

	dataVector.push_back(ptr);
	dataLenVector.push_back(len);
	userData.push_back(user_data);
}

void _LibVectorData::Clear(void)
{
	for (u32 i=0; i<dataVector.size(); i++) {
		free(dataVector[i]);
	}
	dataVector.clear();
	dataLenVector.clear();
	userData.clear();
}

void _LibMenu_Demo(void)
{
	_LibVectorMenu obj;
	char str[] = "abcdef";
	

	obj.Add(str, 3);
	obj.Add(str, 6);

	vector<string>::iterator it = obj.strVector.begin();

	cout << "AAA" << *it << '\n';
	it++;
	cout << "AAA" << obj.strVector[1] << '\n';

	printf("%s\n", obj.strVector[1].c_str());

	DUMPD(obj.menuIndex.curr);
	DUMPD(obj.strVector.size());

	obj.Clear();
	DUMPD(obj.menuIndex.max);
}
