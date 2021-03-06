# define _CRT_SECURE_NO_WARNINGS
# include "PE.h"

int GetFileLength(FILE *pf, DWORD *Length)
{
	int ret = 0;

	fseek(pf, 0, SEEK_END);
	*Length = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	return ret;
}

int MyReadFile(void **pFileAddress)
{
	int ret = 0;
	DWORD Length = 0;
	//打开文件
    FILE* pf = fopen(*pFileAddress, "rb");
	if (pf == NULL)
	{
		ret = -1;
		printf("func ReadFile() Error!\n");
		return ret;
	}

	//获取文件长度
	ret = GetFileLength(pf, &Length);
	if (ret != 0 && Length == -1)
	{
		ret = -2;
		printf("func GetFileLength() Error!\n");
		return ret;
	}

	//分配空间
	*pFileAddress = (PVOID)malloc(Length);
	if (*pFileAddress == NULL)
	{
		ret = -3;
		printf("func malloc() Error!\n");
		return ret;
	}
	memset(*pFileAddress, 0, Length);

	//读取文件进入内存
	fread(*pFileAddress, Length, 1, pf);

	fclose(pf);
	return ret;
}


int MyReadFile_V2(void** pFileAddress, PCHAR FilePath)
{
	int ret = 0;
	DWORD Length = 0;
	//打开文件
	FILE* pf = fopen(FilePath, "rb");
	if (pf == NULL)
	{
		ret = -1;
		printf("func ReadFile() Error!\n");
		return ret;
	}

	//获取文件长度
	ret = GetFileLength(pf, &Length);
	if (ret != 0 && Length == -1)
	{
		ret = -2;
		printf("func GetFileLength() Error!\n");
		return ret;
	}

	//分配空间
	*pFileAddress = (PVOID)malloc(Length);
	if (*pFileAddress == NULL)
	{
		ret = -3;
		printf("func malloc() Error!\n");
		return ret;
	}
	memset(*pFileAddress, 0, Length);

	//读取文件进入内存
	fread(*pFileAddress, Length, 1, pf);

	fclose(pf);
	return ret;
}

int MyWriteFile(PVOID pFileAddress, DWORD FileSize, LPSTR FilePath)
{
	int ret = 0;

	FILE *pf = fopen(FilePath, "wb");
	if (pf == NULL)
	{
		ret = -5;
		printf("func fopen() error :%d!\n", ret);
		return ret;
	}

	fwrite(pFileAddress, FileSize, 1, pf);

	fclose(pf);

	return ret;
}

int FOA_TO_RVA(PVOID FileAddress, DWORD FOA, PDWORD pRVA)
{
	int ret = 0;
    int i;
	PIMAGE_DOS_HEADER pDosHeader				= (PIMAGE_DOS_HEADER)(FileAddress);
	PIMAGE_FILE_HEADER pFileHeader				= (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader	= (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER pSectionGroup			= (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

	//FOA在文件头中 或 SectionAlignment 等于 FileAlignment 时RVA等于FOA
	if (FOA < pOptionalHeader->SizeOfHeaders || pOptionalHeader->SectionAlignment == pOptionalHeader->FileAlignment)
	{
		*pRVA = FOA;
		return ret;
	}

	//FOA在节区中
    for (i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		if (FOA >= pSectionGroup[i].PointerToRawData && FOA < pSectionGroup[i].PointerToRawData + pSectionGroup[i].SizeOfRawData)
		{
			*pRVA = pSectionGroup[i].VirtualAddress + FOA - pSectionGroup[i].PointerToRawData;
			return ret;
		}
	}

	//没有找到地址
	ret = -4;
	printf("func FOA_TO_RAV() Error: %d 地址转换失败！\n", ret);
	return ret;
}

int RVA_TO_FOA(PVOID FileAddress, DWORD RVA, PDWORD pFOA)
{
	int ret = 0;
    int i;
	PIMAGE_DOS_HEADER pDosHeader				= (PIMAGE_DOS_HEADER)(FileAddress);
	PIMAGE_FILE_HEADER pFileHeader				= (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
	PIMAGE_OPTIONAL_HEADER32 pOptionalHeader	= (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER pSectionGroup			= (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

	//RVA在文件头中 或 SectionAlignment 等于 FileAlignment 时RVA等于FOA
	if (RVA < pOptionalHeader->SizeOfHeaders || pOptionalHeader->SectionAlignment == pOptionalHeader->FileAlignment)
	{
		*pFOA = RVA;
		return ret;
	}

	//RVA在节区中
    for (i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		if (RVA >= pSectionGroup[i].VirtualAddress && RVA < pSectionGroup[i].VirtualAddress + pSectionGroup[i].Misc.VirtualSize)
		{
			*pFOA = pSectionGroup[i].PointerToRawData + RVA - pSectionGroup[i].VirtualAddress;
			return ret;
		}
	}

	//没有找到地址
	ret = -4;
	printf("func RAV_TO_FOA() Error: %d 地址转换失败！\n", ret);
	return ret;
}

int PrintEXEDosHeader_V2(PVOID pFileAddress)
{
    int ret = 0;
    int i;
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileAddress;


    printf("****************DOS_Header STAR*************************\n");
    printf("Dos->e_magic        :%02X\n", pDosHeader->e_magic);
    printf("Dos->e_cblp         :%02X\n", pDosHeader->e_cblp);
    printf("Dos->e_cp           :%02X\n", pDosHeader->e_cp);
    printf("Dos->e_crlc         :%02X\n", pDosHeader->e_crlc);
    printf("Dos->e_aparhdr      :%02X\n", pDosHeader->e_cparhdr);
    printf("Dos->e_minalloc     :%02X\n", pDosHeader->e_minalloc);
    printf("Dos->e_maxalloc     :%02X\n", pDosHeader->e_maxalloc);
    printf("Dos->e_ss           :%02X\n", pDosHeader->e_ss);
    printf("Dos->e_sp           :%02X\n", pDosHeader->e_sp);
    printf("Dos->e_csum         :%02X\n", pDosHeader->e_csum);
    printf("Dos->e_ip           :%02X\n", pDosHeader->e_ip);
    printf("Dos->e_cs           :%02X\n", pDosHeader->e_cs);
    printf("Dos->e_lfarlc       :%02X\n", pDosHeader->e_lfarlc);
    printf("Dos->e_ovno         :%02X\n", pDosHeader->e_ovno);
    for (i = 0; i < 4; i++)
    {
        printf("Dos->e_res[%d]       :%02X\n", i, pDosHeader->e_res[i]);
    }
    printf("Dos->e_oeminfo      :%02X\n", pDosHeader->e_oeminfo);
    for (i = 0; i < 10; i++)
    {
        printf("Dos->e_res2[%d]      :%02X\n", i, pDosHeader->e_res2[i]);
    }
    printf("Dos->e_lfanew       :%04X\n", pDosHeader->e_lfanew);
    printf("*****************DOS_Header END************************\n");

    return ret;
}

int PrintEXEFileHeader_V2(PVOID pFileAddress)
{
    int ret = 0;
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileAddress;
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + (DWORD)pDosHeader->e_lfanew + 4);

    printf("****************FILE_HEADER STAR*************************\n");
    printf("FileHeader->Machine              : %02X\n", pFileHeader->Machine);
    printf("FileHeader->NumberOfSections     : %02X\n", pFileHeader->NumberOfSections);
    printf("FileHeader->TimeDateStamp        : %04X\n", pFileHeader->TimeDateStamp);
    printf("FileHeader->PointerToSymbolTable : %04X\n", pFileHeader->PointerToSymbolTable);
    printf("FileHeader->NumberOfSymbols      : %04X\n", pFileHeader->NumberOfSymbols);
    printf("FileHeader->SizeOfOptionalHeader : %02X\n", pFileHeader->SizeOfOptionalHeader);
    printf("FileHeader->Characteristics      : %02X\n", pFileHeader->Characteristics);

    printf("*****************FILE_HEADER END************************\n");

    return ret;
}

int PrintEXEOptionalHeader_V2(PVOID pFileAddress)
{
    int ret = 0;

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileAddress;
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + (DWORD)pDosHeader->e_lfanew + 4);
    PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));


    printf("****************OPTIONAL_HEADER32 STAR*************************\n");
    printf("OptionalHeader->Magic                        : %02X\n", pOptionalHeader->Magic);
    printf("OptionalHeader->MajorLinkerVersion           : %01X\n", pOptionalHeader->MajorLinkerVersion);
    printf("OptionalHeader->MinorLinkerVersion           : %01X\n", pOptionalHeader->MinorLinkerVersion);
    printf("OptionalHeader->SizeOfCode                   : %04X\n", pOptionalHeader->SizeOfCode);
    printf("OptionalHeader->SizeOfInitializedData        : %04X\n", pOptionalHeader->SizeOfInitializedData);
    printf("OptionalHeader->SizeOfUninitializedData      : %04X\n", pOptionalHeader->SizeOfUninitializedData);
    printf("OptionalHeader->AddressOfEntryPoint          : %04X\n", pOptionalHeader->AddressOfEntryPoint);
    printf("OptionalHeader->BaseOfCode                   : %04X\n", pOptionalHeader->BaseOfCode);
    printf("OptionalHeader->BaseOfData                   : %04X\n", pOptionalHeader->BaseOfData);
    printf("OptionalHeader->ImageBase                    : %04X\n", pOptionalHeader->ImageBase);
    printf("OptionalHeader->SectionAlignment             : %04X\n", pOptionalHeader->SectionAlignment);
    printf("OptionalHeader->FileAlignment                : %04X\n", pOptionalHeader->FileAlignment);
    //printf("OptionalHeader->MajorOEXEratingSystemVersion  : %02X\n", pOptionalHeader->MajorOEXEratingSystemVersion);
    //printf("OptionalHeader->MinorOEXEratingSystemVersion  : %02X\n", pOptionalHeader->MinorOEXEratingSystemVersion);
    printf("OptionalHeader->MajorImageVersion            : %02X\n", pOptionalHeader->MajorImageVersion);
    printf("OptionalHeader->MinorImageVersion            : %02X\n", pOptionalHeader->MinorImageVersion);
    printf("OptionalHeader->MajorSubsystemVersion        : %02X\n", pOptionalHeader->MajorSubsystemVersion);
    printf("OptionalHeader->MinorSubsystemVersion        : %02X\n", pOptionalHeader->MinorSubsystemVersion);
    printf("OptionalHeader->Win32VersionValue            : %04X\n", pOptionalHeader->Win32VersionValue);
    printf("OptionalHeader->SizeOfImage                  : %04X\n", pOptionalHeader->SizeOfImage);
    printf("OptionalHeader->SizeOfHeaders                : %04X\n", pOptionalHeader->SizeOfHeaders);
    printf("OptionalHeader->CheckSum                     : %04X\n", pOptionalHeader->CheckSum);
    printf("OptionalHeader->Subsystem                    : %02X\n", pOptionalHeader->Subsystem);
    printf("OptionalHeader->DllCharacteristics           : %02X\n", pOptionalHeader->DllCharacteristics);
    printf("OptionalHeader->SizeOfStackReserv            : %04X\n", pOptionalHeader->SizeOfStackReserve);
    printf("OptionalHeader->SizeOfStackCommit            : %04X\n", pOptionalHeader->SizeOfStackCommit);
    printf("OptionalHeader->SizeOfHeapReserve            : %04X\n", pOptionalHeader->SizeOfHeapReserve);
    printf("OptionalHeader->SizeOfHeapCommit             : %04X\n", pOptionalHeader->SizeOfHeapCommit);
    printf("OptionalHeader->LoaderFlags                  : %04X\n", pOptionalHeader->LoaderFlags);
    printf("OptionalHeader->NumberOfRvaAndSizes          : %04X\n", pOptionalHeader->NumberOfRvaAndSizes);

    printf("*****************OPTIONAL_HEADER32 END************************\n");

    return ret;
}

int PrintEXESectionHeader_V2(PVOID pFileAddress)
{
    int ret = 0;
    int i;
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileAddress;
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + (DWORD)pDosHeader->e_lfanew + 4);
    PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));
    PIMAGE_SECTION_HEADER pSectionGroup = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

    printf("****************SECTION_HEADER STAR*************************\n");
    for (i = 0; i < pFileHeader->NumberOfSections; i++)
    {
        printf("pSectionGroup[%d].Name                   : %s\n", i, pSectionGroup[i].Name);
        printf("pSectionGroup[%d].Misc.VirtualSize       : %04X\n", i, pSectionGroup[i].Misc.VirtualSize);
        printf("pSectionGroup[%d].VirtualAddress         : %04X\n", i, pSectionGroup[i].VirtualAddress);
        printf("pSectionGroup[%d].SizeOfRawData          : %04X\n", i, pSectionGroup[i].SizeOfRawData);
        printf("pSectionGroup[%d].PointerToRawData       : %04X\n", i, pSectionGroup[i].PointerToRawData);
        printf("pSectionGroup[%d].PointerToRelocations   : %04X\n", i, pSectionGroup[i].PointerToRelocations);
        printf("pSectionGroup[%d].PointerToLinenumbers   : %04X\n", i, pSectionGroup[i].PointerToLinenumbers);
        printf("pSectionGroup[%d].NumberOfRelocations    : %02X\n", i, pSectionGroup[i].NumberOfRelocations);
        printf("pSectionGroup[%d].NumberOfLinenumbers    : %02X\n", i, pSectionGroup[i].NumberOfLinenumbers);
        printf("pSectionGroup[%d].Characteristics        : %04X\n\n\n", i, pSectionGroup[i].Characteristics);
    }

    printf("*****************SECTION_HEADER END************************\n");

    return ret;
}
int PrintEXEDirectory_V2(PVOID pFileAddress)
{
    int ret = 0;
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileAddress;
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + (DWORD)pDosHeader->e_lfanew + 4);
    PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));


    printf("****************Directory STAR*************************\n");
    printf("|名称：      |RVA地址：        |大小：\n");
    printf("|------------+-----------------+------------\n");
    printf("|导出表      |%08X         |%08X\n", pOptionalHeader->DataDirectory[0].VirtualAddress, pOptionalHeader->DataDirectory[0].Size);
    printf("|------------+-----------------+------------\n");
    printf("|导入表      |%08X         |%08X\n", pOptionalHeader->DataDirectory[1].VirtualAddress, pOptionalHeader->DataDirectory[1].Size);
    printf("|------------+-----------------+------------\n");
    printf("|资源表      |%08X         |%08X\n", pOptionalHeader->DataDirectory[2].VirtualAddress, pOptionalHeader->DataDirectory[2].Size);
    printf("|------------+-----------------+------------\n");
    printf("|异常信息    |%08X         |%08X\n", pOptionalHeader->DataDirectory[3].VirtualAddress, pOptionalHeader->DataDirectory[3].Size);
    printf("|------------+-----------------+------------\n");
    printf("|安全证书    |%08X         |%08X\n", pOptionalHeader->DataDirectory[4].VirtualAddress, pOptionalHeader->DataDirectory[4].Size);
    printf("|------------+-----------------+------------\n");
    printf("|重定位表    |%08X         |%08X\n", pOptionalHeader->DataDirectory[5].VirtualAddress, pOptionalHeader->DataDirectory[5].Size);
    printf("|------------+-----------------+------------\n");
    printf("|调试信息    |%08X         |%08X\n", pOptionalHeader->DataDirectory[6].VirtualAddress, pOptionalHeader->DataDirectory[6].Size);
    printf("|------------+-----------------+------------\n");
    printf("|版权所有    |%08X         |%08X\n", pOptionalHeader->DataDirectory[7].VirtualAddress, pOptionalHeader->DataDirectory[7].Size);
    printf("|------------+-----------------+------------\n");
    printf("|全局指针    |%08X         |%08X\n", pOptionalHeader->DataDirectory[8].VirtualAddress, pOptionalHeader->DataDirectory[8].Size);
    printf("|------------+-----------------+------------\n");
    printf("|TLS表       |%08X         |%08X\n", pOptionalHeader->DataDirectory[9].VirtualAddress, pOptionalHeader->DataDirectory[9].Size);
    printf("|------------+-----------------+------------\n");
    printf("|加载配置    |%08X         |%08X\n", pOptionalHeader->DataDirectory[10].VirtualAddress, pOptionalHeader->DataDirectory[10].Size);
    printf("|------------+-----------------+------------\n");
    printf("|绑定导入    |%08X         |%08X\n", pOptionalHeader->DataDirectory[11].VirtualAddress, pOptionalHeader->DataDirectory[11].Size);
    printf("|------------+-----------------+------------\n");
    printf("|IAT表       |%08X         |%08X\n", pOptionalHeader->DataDirectory[12].VirtualAddress, pOptionalHeader->DataDirectory[12].Size);
    printf("|------------+-----------------+------------\n");
    printf("|延迟导入    |%08X         |%08X\n", pOptionalHeader->DataDirectory[13].VirtualAddress, pOptionalHeader->DataDirectory[13].Size);
    printf("|------------+-----------------+------------\n");
    printf("|COM表       |%08X         |%08X\n", pOptionalHeader->DataDirectory[14].VirtualAddress, pOptionalHeader->DataDirectory[14].Size);
    printf("|------------+-----------------+------------\n");
    printf("|保留项      |%08X         |%08X\n", pOptionalHeader->DataDirectory[15].VirtualAddress, pOptionalHeader->DataDirectory[15].Size);
    printf("|------------+-----------------+------------\n");
    printf("*****************Directory END************************\n");

    return ret;
}

int PrintFunctionAddressTable_V2(PVOID FileAddress, DWORD AddressOfFunctions_RVA, DWORD NumberOfFunctions)
{
    int ret = 0;
    int i;
    DWORD AddressOfFunctions_FOA = 0;

    //1、RVA --> FOA
    ret = RVA_TO_FOA(FileAddress, AddressOfFunctions_RVA, &AddressOfFunctions_FOA);
    if (ret != 0)
    {
        printf("func RVA_TO_FOA() Error!\n");
        return ret;
    }

    //2、指向函数地址表
    PDWORD FuncAddressTable = (PDWORD)((DWORD)FileAddress + AddressOfFunctions_FOA);

    //2、循环打印函数地址表
    printf("=================== 函数地址表 Start ===================\n");
    for (i = 0; i < NumberOfFunctions; i++)
    {
        DWORD FuncAddress_RVA = FuncAddressTable[i];
        DWORD FuncAddress_FOA = 0;
        ret = RVA_TO_FOA(FileAddress, FuncAddress_RVA, &FuncAddress_FOA);
        if (ret != 0)
        {
            printf("func RVA_TO_FOA() Error!\n");
            return ret;
        }

        printf("函数地址RVA    : %08X  |函数地址FOA    : %08X  \n", FuncAddress_RVA, FuncAddress_FOA);
    }
    printf("=================== 函数地址表 End   ===================\n\n");
    return ret;
}

//打印函数序号表
int PrintFunctionOrdinalTable_V2(PVOID FileAddress, DWORD AddressOfOrdinal_RVA, DWORD NumberOfNames, DWORD Base)
{
    int ret = 0;
    int i;
    DWORD AddressOfOrdinal_FOA = 0;

    //1、RVA --> FOA
    ret = RVA_TO_FOA(FileAddress, AddressOfOrdinal_RVA, &AddressOfOrdinal_FOA);
    if (ret != 0)
    {
        printf("func RVA_TO_FOA() Error!\n");
        return ret;
    }

    //2、指向函数序号表
    PWORD OrdinalTable = (PWORD)((DWORD)FileAddress + AddressOfOrdinal_FOA);

    //3、循环打印函数序号表
    printf("=================== 函数序号表 Start ===================\n");
    for (i = 0; i < NumberOfNames; i++)
    {
        printf("函数序号  :%04X  |Base+Ordinal   :%04X\n", OrdinalTable[i], OrdinalTable[i] + Base);
    }
    printf("=================== 函数序号表 End   ===================\n\n");
    return ret;
}


int PrintFunctionNameTable_V2(PVOID FileAddress, DWORD AddressOfNames_RVA, DWORD NumberOfNames)
{
    int ret = 0;
    int i;
    DWORD AddressOfNames_FOA = 0;

    //1、RVA --> FOA
    ret = RVA_TO_FOA(FileAddress, AddressOfNames_RVA, &AddressOfNames_FOA);
    if (ret != 0)
    {
        printf("func RVA_TO_FOA() Error!\n");
        return ret;
    }

    //2、指向函数名表
    PDWORD NameTable = (PDWORD)((DWORD)FileAddress + AddressOfNames_FOA);

    //3、循环打印函数序号表
    printf("=================== 函数名表 Start ===================\n");
    for (i = 0; i < NumberOfNames; i++)
    {
        DWORD FuncName_RVA = NameTable[i];
        DWORD FuncName_FOA = 0;
        ret = RVA_TO_FOA(FileAddress, FuncName_RVA, &FuncName_FOA);
        if (ret != 0)
        {
            printf("func RVA_TO_FOA() Error!\n");
            return ret;
        }
        PCHAR FuncName = (PCHAR)((DWORD)FileAddress + FuncName_FOA);

        printf("函数名  :%s\n", FuncName);
    }
    printf("=================== 函数名表 End   ===================\n\n");

    return ret;
}

int PrintExportTable_V2(PVOID FileAddress)
{
    int ret = 0;

    //1、指向相关内容
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)(FileAddress);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
    PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));

    //2、获取导出表的地址
    DWORD ExportDirectory_RAVAdd = pOptionalHeader->DataDirectory[0].VirtualAddress;
    DWORD ExportDirectory_FOAAdd = 0;
    //	(1)、判断导出表是否存在
    if (ExportDirectory_RAVAdd == 0)
    {
        printf("ExportDirectory 不存在!\n");
        return ret;
    }
    //	(2)、获取导出表的FOA地址
    ret = RVA_TO_FOA(FileAddress, ExportDirectory_RAVAdd, &ExportDirectory_FOAAdd);
    if (ret != 0)
    {
        printf("func RVA_TO_FOA() Error!\n");
        return ret;
    }

    //3、指向导出表
    PIMAGE_EXPORT_DIRECTORY ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((DWORD)FileAddress + ExportDirectory_FOAAdd);

    //4、找到文件名
    DWORD FileName_RVA = ExportDirectory->Name;
    DWORD FileName_FOA = 0;
    ret = RVA_TO_FOA(FileAddress, FileName_RVA, &FileName_FOA);
    if (ret != 0)
    {
        printf("func RVA_TO_FOA() Error!\n");
        return ret;
    }
    PCHAR FileName = (PCHAR)((DWORD)FileAddress + FileName_FOA);

    //5、打印导出表信息
    printf("DWORD Characteristics;        :  %08X\n", ExportDirectory->Characteristics);
    printf("DWORD TimeDateStamp;          :  %08X\n", ExportDirectory->TimeDateStamp);
    printf("WORD  MajorVersion;           :  %04X\n", ExportDirectory->MajorVersion);
    printf("WORD  MinorVersion;           :  %04X\n", ExportDirectory->MinorVersion);
    printf("DWORD Name;                   :  %08X     \"%s\"\n", ExportDirectory->Name, FileName);
    printf("DWORD Base;                   :  %08X\n", ExportDirectory->Base);
    printf("DWORD NumberOfFunctions;      :  %08X\n", ExportDirectory->NumberOfFunctions);
    printf("DWORD NumberOfNames;          :  %08X\n", ExportDirectory->NumberOfNames);
    printf("DWORD AddressOfFunctions;     :  %08X\n", ExportDirectory->AddressOfFunctions);
    printf("DWORD AddressOfNames;         :  %08X\n", ExportDirectory->AddressOfNames);
    printf("DWORD AddressOfNameOrdinals;  :  %08X\n", ExportDirectory->AddressOfNameOrdinals);
    printf("=========================================================\n");
    printf("*********************************************************\n");

    //6、打印函数地址表 数量由NumberOfFunctions决定
    ret = PrintFunctionAddressTable_V2(FileAddress, ExportDirectory->AddressOfFunctions, ExportDirectory->NumberOfFunctions);
    if (ret != 0)
    {
        printf("func PrintFunctionAddressTable() Error!\n");
        return ret;
    }

    //7、打印函数序号表 数量由NumberOfNames决定
    ret = PrintFunctionOrdinalTable_V2(FileAddress, ExportDirectory->AddressOfNameOrdinals, ExportDirectory->NumberOfNames, ExportDirectory->Base);
    if (ret != 0)
    {
        printf("func PrintFunctionOrdinalTable() Error!\n");
        return ret;
    }

    //8、打印函数名表 数量由NumberOfNames决定
    ret = PrintFunctionNameTable_V2(FileAddress, ExportDirectory->AddressOfNames, ExportDirectory->NumberOfNames);
    if (ret != 0)
    {
        printf("func PrintFunctionNameTable() Error!\n");
        return ret;
    }

    return ret;
}



int PrintReloactionTable_V2(PVOID FileAddress)
{
    int ret = 0;
    int i;
    //1、指向相关内容
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)(FileAddress);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
    PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));

    //2、获取重定位表的地址
    DWORD RelocationDirectory_RAVAdd = pOptionalHeader->DataDirectory[5].VirtualAddress;
    DWORD RelocationDirectory_FOAAdd = 0;
    //	(1)、判断重定位表是否存在
    if (RelocationDirectory_RAVAdd == 0)
    {
        printf("RelocationDirectory 不存在!\n");
        return ret;
    }
    //	(2)、获取重定位表的FOA地址
    ret = RVA_TO_FOA(FileAddress, RelocationDirectory_RAVAdd, &RelocationDirectory_FOAAdd);
    if (ret != 0)
    {
        printf("func RVA_TO_FOA() Error!\n");
        return ret;
    }

    //3、指向重定位表
    PIMAGE_BASE_RELOCATION RelocationDirectory = (PIMAGE_BASE_RELOCATION)((DWORD)FileAddress + RelocationDirectory_FOAAdd);

    //4、循环打印重定位信息  当VirtualAddress和SizeOfBlock都为0时遍历完成
    while (RelocationDirectory->VirtualAddress && RelocationDirectory->SizeOfBlock)
    {
        printf("VirtualAddress    :%08X\n", RelocationDirectory->VirtualAddress);
        printf("SizeOfBlock       :%08X\n", RelocationDirectory->SizeOfBlock);
        printf("================= BlockData Start ======================\n");
        //5、计算在当前块中的数据个数
        DWORD DataNumber = (RelocationDirectory->SizeOfBlock - 8) / 2;

        //6、指向数据块
        PWORD DataGroup = (PWORD)((DWORD)RelocationDirectory + 8);

        //7、循环打印数据块中的数据
        for (i = 0; i < DataNumber; i++)
        {
            //(1)判断高4位是否为0
            if (DataGroup[i] >> 12 != 0)
            {
                //(2)提取数据块中的有效数据 低12位
                WORD BlockData = DataGroup[i] & 0xFFF;

                //(3)计算数据块的RVA和FOA
                DWORD Data_RVA = RelocationDirectory->VirtualAddress + BlockData;
                DWORD Data_FOA = 0;
                ret = RVA_TO_FOA(FileAddress, Data_RVA, &Data_FOA);
                if (ret != 0)
                {
                    printf("func RVA_TO_FOA() Error!\n");
                    return ret;
                }

                //(4)获取需要重定位的数据
                PDWORD RelocationData = (PDWORD)((DWORD)FileAddress + Data_FOA);

                printf("第[%04X]项    |数据 :[%04X]   |数据的RVA :[%08X]  |数据属性 :[%X]  |重定位数据  :[%08X]\n", i + 1, BlockData, Data_RVA, (DataGroup[i] >> 12), *RelocationData);
            }
        }
        printf("================= BlockData End ========================\n");

        //指向下一个数据块
        RelocationDirectory = (PIMAGE_BASE_RELOCATION)((DWORD)RelocationDirectory + RelocationDirectory->SizeOfBlock);
    }

    return ret;
}



int PrintImportTable_V2(PVOID FileAddress)
{
    int ret = 0;
    //1、指向相关内容
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)(FileAddress);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
    PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));

    //2、获取导入表的地址
    DWORD ImportDirectory_RVAAdd = pOptionalHeader->DataDirectory[1].VirtualAddress;
    DWORD ImportDirectory_FOAAdd = 0;
    //	(1)、判断导入表是否存在
    if (ImportDirectory_RVAAdd == 0)
    {
        printf("ImportDirectory 不存在!\n");
        return ret;
    }
    //	(2)、获取导入表的FOA地址
    ret = RVA_TO_FOA(FileAddress, ImportDirectory_RVAAdd, &ImportDirectory_FOAAdd);
    if (ret != 0)
    {
        printf("func RVA_TO_FOA() Error!\n");
        return ret;
    }

    //3、指向导入表
    PIMAGE_IMPORT_DESCRIPTOR ImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)FileAddress + ImportDirectory_FOAAdd);

    //4、循环打印每一个导入表的信息  重要成员为0时结束循环
    while (ImportDirectory->FirstThunk && ImportDirectory->OriginalFirstThunk)
    {
        //	(1)获取导入文件的名字
        DWORD ImportNameAdd_RVA = ImportDirectory->Name;
        DWORD ImportNameAdd_FOA = 0;
        ret = RVA_TO_FOA(FileAddress, ImportNameAdd_RVA, &ImportNameAdd_FOA);
        if (ret != 0)
        {
            printf("func RVA_TO_FOA() Error!\n");
            return ret;
        }
        PCHAR pImportName = (PCHAR)((DWORD)FileAddress + ImportNameAdd_FOA);

        printf("=========================ImportTable %s Start=============================\n", pImportName);
        printf("OriginalFirstThunk RVA:%08X\n", ImportDirectory->OriginalFirstThunk);

        //	(2)指向INT表
        DWORD OriginalFirstThunk_RVA = ImportDirectory->OriginalFirstThunk;
        DWORD OriginalFirstThunk_FOA = 0;
        ret = RVA_TO_FOA(FileAddress, OriginalFirstThunk_RVA, &OriginalFirstThunk_FOA);
        if (ret != 0)
        {
            printf("func RVA_TO_FOA() Error!\n");
            return ret;
        }
        PDWORD OriginalFirstThunk_INT = (PDWORD)((DWORD)FileAddress + OriginalFirstThunk_FOA);

        //	(3)循环打印INT表的内容		当内容为0时结束
        while (*OriginalFirstThunk_INT)
        {
            //	(4)进行判断,如果最高位为1则是按序号导入信息,去掉最高位就是函数序号,否则是名字导入
            if ((*OriginalFirstThunk_INT) >> 31)	//最高位是1,序号导入
            {
                //	(5)获取函数序号
                DWORD Original = *OriginalFirstThunk_INT << 1 >> 1;	//去除最高标志位。
                printf("按序号导入: %08Xh -- %08dd\n", Original, Original);	//16进制 -- 10 进制
            }
            else	//名字导入
            {
                //	(5)获取函数名
                DWORD ImportNameAdd_RAV = *OriginalFirstThunk_INT;
                DWORD ImportNameAdd_FOA = 0;
                ret = RVA_TO_FOA(FileAddress, ImportNameAdd_RAV, &ImportNameAdd_FOA);
                if (ret != 0)
                {
                    printf("func RVA_TO_FOA() Error!\n");
                    return ret;
                }
                PIMAGE_IMPORT_BY_NAME ImportName = (PIMAGE_IMPORT_BY_NAME)((DWORD)FileAddress + ImportNameAdd_FOA);
                printf("按名字导入[HINT/NAME]: %02X--%s\n", ImportName->Hint, ImportName->Name);
            }

            //	(6)指向下一个地址
            OriginalFirstThunk_INT++;
        }
        printf("----------------------------------------------------------------\n");
        printf("FirstThunk RVA   :%08X\n", ImportDirectory->FirstThunk);
        printf("TimeDateStamp    :%08X\n", ImportDirectory->TimeDateStamp);

        //	(2)指向IAT表
        DWORD FirstThunk_RVA = ImportDirectory->FirstThunk;
        DWORD FirstThunk_FOA = 0;
        ret = RVA_TO_FOA(FileAddress, FirstThunk_RVA, &FirstThunk_FOA);
        if (ret != 0)
        {
            printf("func RVA_TO_FOA() Error!\n");
            return ret;
        }
        PDWORD FirstThunk_IAT = (PDWORD)((DWORD)FileAddress + FirstThunk_FOA);

        //	(3)判断IAT表是否被绑定	时间戳 = 0:没有绑定地址	时间戳 = 0xFFFFFFFF:绑定地址	――知识在绑定导入表中
        if (ImportDirectory->TimeDateStamp == 0xFFFFFFFF)
        {
            while (*FirstThunk_IAT)
            {
                printf("绑定函数地址: %08X\n", *FirstThunk_IAT);
                FirstThunk_IAT++;
            }
        }
        else
        {
            //	(4)循环打印IAT表的内容		当内容为0时结束	打印方法和INT表一样
            while (*FirstThunk_IAT)
            {
                //	(5)进行判断,如果最高位为1则是按序号导入信息,去掉最高位就是函数序号,否则是名字导入
                if ((*FirstThunk_IAT) >> 31)	//最高位是1,序号导入
                {
                    //	(6)获取函数序号
                    DWORD Original = *FirstThunk_IAT << 1 >> 1;	//去除最高标志位。
                    printf("按序号导入: %08Xh -- %08dd\n", Original, Original);	//16进制 -- 10 进制
                }
                else	//名字导入
                {
                    //	(7)获取函数名
                    DWORD ImportNameAdd_RAV = *FirstThunk_IAT;
                    DWORD ImportNameAdd_FOA = 0;
                    ret = RVA_TO_FOA(FileAddress, ImportNameAdd_RAV, &ImportNameAdd_FOA);
                    if (ret != 0)
                    {
                        printf("func RVA_TO_FOA() Error!\n");
                        return ret;
                    }
                    PIMAGE_IMPORT_BY_NAME ImportName = (PIMAGE_IMPORT_BY_NAME)((DWORD)FileAddress + ImportNameAdd_FOA);
                    printf("按名字导入[HINT/NAME]: %02X--%s\n", ImportName->Hint, ImportName->Name);
                }

                FirstThunk_IAT++;
            }

        }

        printf("=========================ImportTable %s End  =============================\n", pImportName);

        //	(8)指向下一个导入表
        ImportDirectory++;
    }

    return ret;
}



int PrintBoundImportTable_V2(PVOID FileAddress)
{
    int ret = 0;
    int i;
    //1、指向相关内容
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)(FileAddress);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
    PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));

    //2、获取绑定导入表的地址
    DWORD BoundImportDirectory_RVAAdd = pOptionalHeader->DataDirectory[11].VirtualAddress;
    DWORD BoundImportDirectory_FOAAdd = 0;
    //	(1)、判断绑定导入表是否存在
    if (BoundImportDirectory_RVAAdd == 0)
    {
        printf("BoundImportDirectory 不存在!\n");
        return ret;
    }
    //	(2)、获取绑定导入表的FOA地址
    ret = RVA_TO_FOA(FileAddress, BoundImportDirectory_RVAAdd, &BoundImportDirectory_FOAAdd);
    if (ret != 0)
    {
        printf("func RVA_TO_FOA() Error!\n");
        return ret;
    }

    //3、指向绑定导入表
    PIMAGE_BOUND_IMPORT_DESCRIPTOR BoundImportDirectory = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)((DWORD)FileAddress + BoundImportDirectory_FOAAdd);


    //4、获取绑定导入表的基址
    DWORD BaseBoundImport = (DWORD)BoundImportDirectory;

    //5、循环打印绑定导入表信息
    while (BoundImportDirectory->OffsetModuleName && BoundImportDirectory->TimeDateStamp)
    {
        //	1)指向模块名
        PCHAR pModuleName = (PCHAR)(BaseBoundImport + BoundImportDirectory->OffsetModuleName);

        //	2)打印绑定导入表信息
        printf("ModuleName                  :%s\n", pModuleName);
        printf("TimeDateStamp               :%08X\n", BoundImportDirectory->TimeDateStamp);
        printf("NumberOfModuleForwarderRefs :%04X\n", BoundImportDirectory->NumberOfModuleForwarderRefs);
        printf("================ Start =========================\n");
        //	3)循环后续结构
        for (i = 0; i < BoundImportDirectory->NumberOfModuleForwarderRefs; i++)
        {
            //	4)指向后续结构
            PIMAGE_BOUND_FORWARDER_REF BoundImport_Ref = (PIMAGE_BOUND_FORWARDER_REF)&BoundImportDirectory[i + 1];//两个结构大小一样

            //	5)指向模块名
            pModuleName = (PCHAR)(BaseBoundImport + BoundImport_Ref->OffsetModuleName);

            //	6)打印信息
            printf("ModuleName-----------:%s\n", pModuleName);
            printf("TimeDateStamp--------:%08X\n\n", BoundImport_Ref->TimeDateStamp);

        }
        printf("================  End  =========================\n");

        //	7)指向下一个结构
        BoundImportDirectory = &BoundImportDirectory[BoundImportDirectory->NumberOfModuleForwarderRefs + 1];
    }

    return ret;
}


PCHAR szResTyEXE_V2[0x11] = { 0, "鼠标指针", "位图", "图标", "菜单",
                              "对话框", "字符串列表","字体目录", "字体",
                              "加速键", "非格式化资源", "消息列表", "鼠标指针组",
                              "zz", "图标组","xx", "版本信息" };

int PrintResourceTable_V2(PVOID FileAddress)
{
    int ret = 0;
    int i,j,k;
    //1、指向相关内容
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)(FileAddress);
    PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pDosHeader + pDosHeader->e_lfanew + 4);
    PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = (PIMAGE_OPTIONAL_HEADER32)((DWORD)pFileHeader + sizeof(IMAGE_FILE_HEADER));

    //2、获取资源表的地址
    DWORD ResourceDirectory_RVAAdd = pOptionalHeader->DataDirectory[2].VirtualAddress;
    DWORD ResourceDirectory_FOAAdd = 0;
    //	(1)、判断资源表是否存在
    if (ResourceDirectory_RVAAdd == 0)
    {
        printf("ResourceDirectory 不存在!\n");
        return ret;
    }
    //	(2)、获取资源表的FOA地址
    ret = RVA_TO_FOA(FileAddress, ResourceDirectory_RVAAdd, &ResourceDirectory_FOAAdd);
    if (ret != 0)
    {
        printf("func RVA_TO_FOA() Error!\n");
        return ret;
    }

    //3、指向资源表
    PIMAGE_RESOURCE_DIRECTORY ResourceDirectory = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)FileAddress + ResourceDirectory_FOAAdd);

    //4、打印资源表信息(一级目录)
    printf("|==================================================\n");
    printf("|资源表一级目录信息:\n");
    printf("|Characteristics        :%08X\n", ResourceDirectory->Characteristics);
    printf("|TimeDateStamp          :%08X\n", ResourceDirectory->TimeDateStamp);
    printf("|MajorVersion           :%04X\n", ResourceDirectory->MajorVersion);
    printf("|MinorVersion           :%04X\n", ResourceDirectory->MinorVersion);
    printf("|NumberOfNamedEntries   :%04X\n", ResourceDirectory->NumberOfNamedEntries);
    printf("|NumberOfIdEntries      :%04X\n", ResourceDirectory->NumberOfIdEntries);
    printf("|==================================================\n");

    //4、循环打印后续资源表信息
    //	(1)指向一级目录中的资源目录项(一级目录)	资源类型
    PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirectoryEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)ResourceDirectory + sizeof(IMAGE_RESOURCE_DIRECTORY));
    printf("|----------------------------------------\n");

    for (i = 0; i < (ResourceDirectory->NumberOfIdEntries + ResourceDirectory->NumberOfNamedEntries); i++)
    {
        //	(2)判断一级目录中的资源目录项中类型是否是字符串 1 = 字符串(非标准类型)； 0 = 非字符串(标准类型)
        if (ResourceDirectoryEntry->NameIsString)		//字符串(非标准类型)
        {
            //		1.指向名字结构体
            PIMAGE_RESOURCE_DIR_STRING_U pStringName = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)ResourceDirectory + ResourceDirectoryEntry->NameOffset);

            //		2.将Unicode字符串转换成ASCII字符串
            CHAR TyEXEName[20] = { 0 };
            for (j = 0; j < pStringName->Length; j++)
            {
                TyEXEName[j] = (CHAR)pStringName->NameString[j];
            }
            //		3.打印字符串
            printf("|ResourceTyEXE           :\"%s\"\n", TyEXEName);

        }
        else		//非字符串(标准类型)
        {
            if (ResourceDirectoryEntry->Id < 0x11)	//只有1 - 16有定义
                printf("|ResourceTyEXE           :%s\n", szResTyEXE_V2[ResourceDirectoryEntry->Id]);
            else
                printf("|ResourceTyEXE           :%04Xh\n", ResourceDirectoryEntry->Id);
        }

        //	(3)判断一级目录中子节点的类型		1 = 目录； 0 = 数据 (一级目录和二级目录该值都为1)
        if (ResourceDirectoryEntry->DataIsDirectory)
        {
            //	(4)打印目录偏移
            printf("|OffsetToDirectory      :%08X\n", ResourceDirectoryEntry->OffsetToDirectory);
            printf("|----------------------------------------\n");

            //	(5)指向二级目录	资源编号
            PIMAGE_RESOURCE_DIRECTORY ResourceDirectory_Sec = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)ResourceDirectory + ResourceDirectoryEntry->OffsetToDirectory);

            //	(6)打印资源表信息(二级目录)
            printf("    |====================================\n");
            printf("    |资源表二级目录信息:\n");
            printf("    |Characteristics        :%08X\n", ResourceDirectory_Sec->Characteristics);
            printf("    |TimeDateStamp          :%08X\n", ResourceDirectory_Sec->TimeDateStamp);
            printf("    |MajorVersion           :%04X\n", ResourceDirectory_Sec->MajorVersion);
            printf("    |MinorVersion           :%04X\n", ResourceDirectory_Sec->MinorVersion);
            printf("    |NumberOfNamedEntries   :%04X\n", ResourceDirectory_Sec->NumberOfNamedEntries);
            printf("    |NumberOfIdEntries      :%04X\n", ResourceDirectory_Sec->NumberOfIdEntries);
            printf("    |====================================\n");

            //	(7)指向二级目录中的资源目录项
            PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirectoryEntry_Sec = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)ResourceDirectory_Sec + sizeof(IMAGE_RESOURCE_DIRECTORY));

            //	(8)循环打印二级目录
            for (j = 0; j < (ResourceDirectory_Sec->NumberOfIdEntries + ResourceDirectory_Sec->NumberOfNamedEntries); j++)
            {
                //	(9)判断二级目录中的资源目录项中编号是否是字符串
                if (ResourceDirectoryEntry_Sec->NameIsString)		//字符串(非标准类型)
                {
                    //		1.指向名字结构体
                    PIMAGE_RESOURCE_DIR_STRING_U pStringName = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)ResourceDirectory + ResourceDirectoryEntry_Sec->NameOffset);

                    //		2.将Unicode字符串转换成ASCII字符串
                    CHAR TyEXEName[20] = { 0 };
                    for (k = 0; k < pStringName->Length; k++)
                    {
                        TyEXEName[k] = (CHAR)pStringName->NameString[k];
                    }
                    //		3.打印字符串
                    printf("    |ResourceNumber         :\"%s\"\n", TyEXEName);
                }
                else		//非字符串(标准类型)
                {
                    printf("    |ResourceNumber         :%04Xh\n", ResourceDirectoryEntry_Sec->Id);
                }

                //	(10)判断二级目录中子节点的类型
                if (ResourceDirectoryEntry_Sec->DataIsDirectory)
                {
                    //	(11)打印目录偏移
                    printf("    |OffsetToDirectory      :%08X\n", ResourceDirectoryEntry_Sec->OffsetToDirectory);
                    printf("    |------------------------------------\n");

                    //	(12)指向三级目录	代码页
                    PIMAGE_RESOURCE_DIRECTORY ResourceDirectory_Thir = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)ResourceDirectory + ResourceDirectoryEntry_Sec->OffsetToDirectory);

                    //	(13)打印资源表信息(三级目录)
                    printf("        |================================\n");
                    printf("        |资源表三级目录信息:\n");
                    printf("        |Characteristics        :%08X\n", ResourceDirectory_Thir->Characteristics);
                    printf("        |TimeDateStamp          :%08X\n", ResourceDirectory_Thir->TimeDateStamp);
                    printf("        |MajorVersion           :%04X\n", ResourceDirectory_Thir->MajorVersion);
                    printf("        |MinorVersion           :%04X\n", ResourceDirectory_Thir->MinorVersion);
                    printf("        |NumberOfNamedEntries   :%04X\n", ResourceDirectory_Thir->NumberOfNamedEntries);
                    printf("        |NumberOfIdEntries      :%04X\n", ResourceDirectory_Thir->NumberOfIdEntries);
                    printf("        |================================\n");

                    //	(14)指向三级目录中的资源目录项
                    PIMAGE_RESOURCE_DIRECTORY_ENTRY ResourceDirectoryEntry_Thir = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)ResourceDirectory_Thir + sizeof(IMAGE_RESOURCE_DIRECTORY));

                    //	(15)循环打印三级目录项
                    for (k = 0; k < (ResourceDirectory_Thir->NumberOfNamedEntries + ResourceDirectory_Thir->NumberOfIdEntries); k++)
                    {
                        //	(16)判断三级目录中的资源目录项中编号是否是字符串
                        if (ResourceDirectoryEntry_Thir->NameIsString)		//字符串(非标准类型)
                        {
                            //		1.指向名字结构体
                            PIMAGE_RESOURCE_DIR_STRING_U pStringName = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)ResourceDirectory + ResourceDirectoryEntry_Thir->NameOffset);

                            //		2.将Unicode字符串转换成ASCII字符串
                            CHAR TyEXEName[20] = { 0 };
                            for (k = 0; k < pStringName->Length; k++)
                            {
                                TyEXEName[k] = (CHAR)pStringName->NameString[k];
                            }
                            //		3.打印字符串
                            printf("        |CodePageNumber         :\"%s\"\n", TyEXEName);
                        }
                        else		//非字符串(标准类型)
                        {
                            printf("        |CodePageNumber         :%04Xh\n", ResourceDirectoryEntry_Thir->Id);
                        }

                        //	(17)判断三级目录中子节点的类型		(三级目录子节点都是数据，这里可以省去判断)
                        if (ResourceDirectoryEntry_Thir->DataIsDirectory)
                        {
                            //	(18)打印偏移
                            printf("        |OffsetToDirectory      :%08X\n", ResourceDirectoryEntry_Thir->OffsetToDirectory);
                            printf("        |------------------------------------\n");
                        }
                        else
                        {
                            //	(18)打印偏移
                            printf("        |OffsetToData           :%08X\n", ResourceDirectoryEntry_Thir->OffsetToData);
                            printf("        |------------------------------------\n");

                            //	(19)指向数据内容	(资源表的FOA + OffsetToData)
                            PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)((DWORD)ResourceDirectory + ResourceDirectoryEntry_Thir->OffsetToData);

                            //	(20)打印数据信息
                            printf("            |================================\n");
                            printf("            |资源表的数据信息\n");
                            printf("            |OffsetToData(RVA)      :%08X\n", ResourceDataEntry->OffsetToData);
                            printf("            |Size                   :%08X\n", ResourceDataEntry->Size);
                            printf("            |CodePage               :%08X\n", ResourceDataEntry->CodePage);
                            printf("            |================================\n");

                        }

                        ResourceDirectoryEntry_Thir++;
                    }
                }
                //	(21)目录项后移
                ResourceDirectoryEntry_Sec++;
            }

        }
        printf("|----------------------------------------\n");
        //	(22)目录项后移
        ResourceDirectoryEntry++;
    }

    return ret;
}

