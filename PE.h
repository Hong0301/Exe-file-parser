# ifndef _PE_H_
# define _PE_H_

# include "stdio.h"
# include "stdlib.h"
# include "windows.h"



#define FILE_PATH "E:/EXE/untitled/sublime_text.exe"
//#define FILE_PATH "E:/EXE/PE/bule.exe"

int GetFileLength(FILE *pf, DWORD *Length);

int MyReadFile(void** pFileAddress);

//***************************************************************************
//MyReadFile:将文件读取到缓冲区
//参数说明：
//pFileAddress 缓冲区地址
//返回值说明：
//成功返回0
//**************************************************************************
int MyReadFile_V2(void** pFileAddress, PCHAR FilePath);

//**************************************************************************
//MyWriteFile:将缓存写入硬盘
//参数说明：
//pFileAddress 缓冲区地址
//FileSize     写入大小
//FilePath     写入路径
//返回值说明：
//成功返回0
//**************************************************************************
int MyWriteFile(PVOID pFileAddress, DWORD FileSize, LPSTR FilePath);

//**************************************************************************
//FOA_TO_RVA:将FOA转换成RVA
//参数说明：
//FileAddress  缓冲区地址
//FOA          FOA值
//pRVA         RVA地址
//返回值说明：
//成功返回0
//**************************************************************************
int FOA_TO_RVA(PVOID FileAddress, DWORD FOA, PDWORD pRVA);

//**************************************************************************
//RVA_TO_FOA:将RVA转换成FOA
//参数说明：
//FileAddress  缓冲区地址
//RVA          RVA值
//pFOA         FOA地址
//返回值说明：
//成功返回0
//**************************************************************************
int RVA_TO_FOA(PVOID FileAddress, DWORD RVA, PDWORD pFOA);

/****************以下都是打印，功能看函数名字*****************/
int PrintEXEDosHeader_V2(PVOID pFileAddress);

int PrintEXEFileHeader_V2(PVOID pFileAddress);

int PrintEXEOptionalHeader_V2(PVOID pFileAddress);

int PrintEXESectionHeader_V2(PVOID pFileAddress);

int PrintEXEDirectory_V2(PVOID pFileAddress);

int PrintFunctionAddressTable_V2(PVOID FileAddress, DWORD AddressOfFunctions_RVA, DWORD NumberOfFunctions);

int PrintFunctionOrdinalTable_V2(PVOID FileAddress, DWORD AddressOfOrdinal_RVA, DWORD NumberOfNames, DWORD Base);

int PrintFunctionNameTable_V2(PVOID FileAddress, DWORD AddressOfNames_RVA, DWORD NumberOfNames);

int PrintExportTable_V2(PVOID FileAddress);

int PrintReloactionTable_V2(PVOID FileAddress);

int PrintImportTable_V2(PVOID FileAddress);

int PrintBoundImportTable_V2(PVOID FileAddress);

int PrintResourceTable_V2(PVOID FileAddress);

# endif
