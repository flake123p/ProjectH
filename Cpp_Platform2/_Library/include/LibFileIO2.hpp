

#ifndef _LIB_FILE_IO_2_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================
typedef enum {
	INI_VAR_STR,
	INI_VAR_DEC,
	INI_VAR_HEX,
} INI_File_Var_Type_t;
typedef struct {
	const char *varStr;
	INI_File_Var_Type_t varType;
	void *valPtr;
} INI_File_Field_t;

void LibFileIO2_Demo_INI_File(void);

#define _LIB_FILE_IO_2_HPP_INCLUDED_
#endif//_LIB_FILE_IO_2_HPP_INCLUDED_

