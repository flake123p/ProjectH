

#ifndef _UNI_VAR_HPP_INCLUDED_

#include "Everything_Lib_Mgr.hpp"
#include "UniVariable.hpp"

class UniVar : public LibAry {
public:
    u32 type; //VAR_IS_UNINITED or ...

    UniVar(void){type=VAR_IS_UNINITED;};
    ~UniVar(void){   };

    int ImportEx(u32 inType, u32 position, const u8 *from, u32 fromLen){type=inType;return Write(position, from, fromLen);};
    int Import(u8 in){return ImportEx(VAR_U8_ARRAY, len, (u8 *)&in, UniVar_GetUnitInBytes(VAR_U8_ARRAY));};
    int Import(u16 in){return ImportEx(VAR_U16_ARRAY, len, (u8 *)&in, UniVar_GetUnitInBytes(VAR_U16_ARRAY));};
    int Import(u32 in){return ImportEx(VAR_U32_ARRAY, len, (u8 *)&in, UniVar_GetUnitInBytes(VAR_U32_ARRAY));};
    int Import(s8 in){return ImportEx(VAR_S8_ARRAY, len, (u8 *)&in, UniVar_GetUnitInBytes(VAR_S8_ARRAY));};
    int Import(s16 in){return ImportEx(VAR_S16_ARRAY, len, (u8 *)&in, UniVar_GetUnitInBytes(VAR_S16_ARRAY));};
    int Import(s32 in){return ImportEx(VAR_S32_ARRAY, len, (u8 *)&in, UniVar_GetUnitInBytes(VAR_S32_ARRAY));};
    int Import(u8 *in, u32 num){return ImportEx(VAR_U8_ARRAY, len, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_U8_ARRAY));};
    int Import(u16 *in, u32 num){return ImportEx(VAR_U16_ARRAY, len, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_U16_ARRAY));};
    int Import(u32 *in, u32 num){return ImportEx(VAR_U32_ARRAY, len, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_U32_ARRAY));};
    int Import(s8 *in, u32 num){return ImportEx(VAR_S8_ARRAY, len, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_S8_ARRAY));};
    int Import(s16 *in, u32 num){return ImportEx(VAR_S16_ARRAY, len, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_S16_ARRAY));};
    int Import(s32 *in, u32 num){return ImportEx(VAR_S32_ARRAY, len, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_S32_ARRAY));};

    //with position
    int Import(u8 in, u32 position){return ImportEx(VAR_U8_ARRAY, position, (u8 *)&in, UniVar_GetUnitInBytes(VAR_U8_ARRAY));};
    int Import(u16 in, u32 position){return ImportEx(VAR_U16_ARRAY, position, (u8 *)&in, UniVar_GetUnitInBytes(VAR_U16_ARRAY));};
    int Import(u32 in, u32 position){return ImportEx(VAR_U32_ARRAY, position, (u8 *)&in, UniVar_GetUnitInBytes(VAR_U32_ARRAY));};
    int Import(s8 in, u32 position){return ImportEx(VAR_S8_ARRAY, position, (u8 *)&in, UniVar_GetUnitInBytes(VAR_S8_ARRAY));};
    int Import(s16 in, u32 position){return ImportEx(VAR_S16_ARRAY, position, (u8 *)&in, UniVar_GetUnitInBytes(VAR_S16_ARRAY));};
    int Import(s32 in, u32 position){return ImportEx(VAR_S32_ARRAY, position, (u8 *)&in, UniVar_GetUnitInBytes(VAR_S32_ARRAY));};
    int Import(u8 *in, u32 num, u32 position){return ImportEx(VAR_U8_ARRAY, position, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_U8_ARRAY));};
    int Import(u16 *in, u32 num, u32 position){return ImportEx(VAR_U16_ARRAY, position, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_U16_ARRAY));};
    int Import(u32 *in, u32 num, u32 position){return ImportEx(VAR_U32_ARRAY, position, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_U32_ARRAY));};
    int Import(s8 *in, u32 num, u32 position){return ImportEx(VAR_S8_ARRAY, position, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_S8_ARRAY));};
    int Import(s16 *in, u32 num, u32 position){return ImportEx(VAR_S16_ARRAY, position, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_S16_ARRAY));};
    int Import(s32 *in, u32 num, u32 position){return ImportEx(VAR_S32_ARRAY, position, (u8 *)in, num*UniVar_GetUnitInBytes(VAR_S32_ARRAY));};

    //string
    int Import(const char *in){u32 num=strlen(in)+1;u32 position = len==0?0:len-1;return ImportEx(VAR_C_STRING, position, (u8 *)in, num);}; //auto concate, overwrite old last '\0'
    int Import(const char *in, u32 position){u32 num=strlen(in)+1;return ImportEx(VAR_C_STRING, position, (u8 *)in, num);};
    int Import(std::string *cppStr, u32 position=0){return ImportEx(VAR_CPP_STRING, position, (u8 *)&cppStr, UniVar_GetUnitInBytes(VAR_VOID_POINTER));};
    //pointer & function
    int Import(void *in){return ImportEx(VAR_VOID_POINTER, len, (u8 *)&in, UniVar_GetUnitInBytes(VAR_VOID_POINTER));};

    u32 MaxNum(void) {
        if (UniVar_GetUnitInBytes(type) == 0) {
            return 0;
        } else {
            return len / UniVar_GetUnitInBytes(type);
        }
    };

    int GetSingleElement32(u32 index, u32 *out);
    int SetSingleElement32(u32 index, u32 in);

    void UniVarDump(void);

    void Reset(void){ResetAndRetainBuffer();};
    void ResetAndRetainBuffer(void){len=0;type=VAR_IS_UNINITED;};
    void DeleteCppString(void){if(type&VAR_IS_CPP_STRING){SAFE_DELETE(*((std::string**)ptr))}Reset();};
};

void UniVar_Demo(void);

#define _UNI_VAR_HPP_INCLUDED_
#endif//_UNI_VAR_HPP_INCLUDED_



