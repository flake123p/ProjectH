# pattern library
typeList  = ['u8 ', 'u16 ', 'u32 ', 's8 ', 's16 ', 's32 ']
typeList2 = ['u8 *', 'u16 *', 'u32 *', 's8 *', 's16 *', 's32 *']
defList  = ['VAR_U8', 'VAR_U16', 'VAR_U32', 'VAR_S8', 'VAR_S16', 'VAR_S32']
defList2 = ['VAR_U8_ARRAY', 'VAR_U16_ARRAY', 'VAR_U32_ARRAY', 'VAR_S8_ARRAY', 'VAR_S16_ARRAY', 'VAR_S32_ARRAY']

# application
#myTypeList = typeList + typeList2
str1 = 'void InitDynamicArray('
str2 = 'in, u32 inAllocIncrement = 1000){ InitDynamicArrayEx('
str3 = ', inAllocIncrement); };'
for i in range(len(typeList)):
	print(str1+typeList[i]+str2+defList2[i]+str3)
for i in range(len(typeList)):
	print(str1+typeList2[i]+str2+defList2[i]+str3)