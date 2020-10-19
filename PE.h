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
//MyReadFile:���ļ���ȡ��������
//����˵����
//pFileAddress ��������ַ
//����ֵ˵����
//�ɹ�����0
//**************************************************************************
int MyReadFile_V2(void** pFileAddress, PCHAR FilePath);

//**************************************************************************
//MyWriteFile:������д��Ӳ��
//����˵����
//pFileAddress ��������ַ
//FileSize     д���С
//FilePath     д��·��
//����ֵ˵����
//�ɹ�����0
//**************************************************************************
int MyWriteFile(PVOID pFileAddress, DWORD FileSize, LPSTR FilePath);

//**************************************************************************
//FOA_TO_RVA:��FOAת����RVA
//����˵����
//FileAddress  ��������ַ
//FOA          FOAֵ
//pRVA         RVA��ַ
//����ֵ˵����
//�ɹ�����0
//**************************************************************************
int FOA_TO_RVA(PVOID FileAddress, DWORD FOA, PDWORD pRVA);

//**************************************************************************
//RVA_TO_FOA:��RVAת����FOA
//����˵����
//FileAddress  ��������ַ
//RVA          RVAֵ
//pFOA         FOA��ַ
//����ֵ˵����
//�ɹ�����0
//**************************************************************************
int RVA_TO_FOA(PVOID FileAddress, DWORD RVA, PDWORD pFOA);

/****************���¶��Ǵ�ӡ�����ܿ���������*****************/
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
