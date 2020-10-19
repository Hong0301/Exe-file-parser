# include "stdio.h"
# include "stdlib.h"
# include "windows.h"
# include "PE.h"
# define _CRT_SECURE_NO_WARNINGS




//=============================================================================================================
//=============================================================================================================
//=============================================================================================================


int main()
{
    int ret = 0;
    PVOID FileAddress = NULL;
    CHAR FilePath[256] = { 0 };
    int Choose = 0;


    printf("请输入EXE文件的路径：");
    scanf("%s", &FilePath);

    //1、将文件读入到内存
    ret = MyReadFile_V2(&FileAddress, FilePath);
    if (ret != 0)
    {
        if (FileAddress != NULL)
            free(FileAddress);
        return ret;
    }

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + (DWORD)pDosHeader->e_lfanew);

    //2、判断是否是EXE文件
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE || pNtHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        ret = -99;
        printf("该文件不是EXE文件!\n");
        return ret;
    }

    //3、选择解析的内容
    while (1)
    {
        system("cls");
        printf("===================================================\n");
        printf("|                欢迎使用小浩的EXE解析器            \n");
        printf("|--------------------------------------------------\n");
        printf("|      1.解析EXE文件头                               \n");
        printf("|      2.解析EXE节表                                 \n");
        printf("|      3.解析EXE目录                                 \n");
        printf("|      4.解析EXE导出表                               \n");
        printf("|      5.解析EXE重定位表                             \n");
        printf("|      6.解析EXE导入表                               \n");
        printf("|      7.解析EXE绑定导入表                            \n");
        printf("|      8.解析EXE资源表                               \n");
        printf("|--------------------------------------------------\n");
        printf("|      输入其他命令退出程序                          \n");
        printf("===================================================\n");
        printf("请选择需要解析的内容：");
        scanf("%d", &Choose);

        switch (Choose)
        {
        case 1:
            system("cls");
            ret = PrintEXEDosHeader_V2(FileAddress);//DOS头
            if (ret != 0)
            {
                if (FileAddress != NULL)
                {
                    free(FileAddress);
                }
                return ret;
            }
            ret = PrintEXEFileHeader_V2(FileAddress);//FileHeader
            if (ret != 0)
            {
                if (FileAddress != NULL)
                {
                    free(FileAddress);
                }
                return ret;
            }
            ret = PrintEXEOptionalHeader_V2(FileAddress);//OptionalHeader
            if (ret != 0)
            {
                if (FileAddress != NULL)
                {
                    free(FileAddress);
                }
                return ret;
            }
            break;

        case 2:
            system("cls");
            ret = PrintEXESectionHeader_V2(FileAddress);//SectionHeader
            if (ret != 0)
            {
                if (FileAddress != NULL)
                {
                    free(FileAddress);
                }
                return ret;
            }
            break;

        case 3:
            system("cls");
            ret = PrintEXEDirectory_V2(FileAddress);
            if (ret != 0)
            {
                if (FileAddress != NULL)
                {
                    free(FileAddress);
                }
                return ret;
            }
            break;

        case 4:
            system("cls");
            ret = PrintExportTable_V2(FileAddress);
            if (ret != 0)
            {
                if (FileAddress != NULL)
                    free(FileAddress);
                return ret;
            }
            break;

        case 5:
            system("cls");
            ret = PrintReloactionTable_V2(FileAddress);
            if (ret != 0)
            {
                if (FileAddress != NULL)
                    free(FileAddress);
                return ret;
            }
            break;

        case 6:
            system("cls");
            ret = PrintImportTable_V2(FileAddress);
            if (ret != 0)
            {
                if (FileAddress != NULL)
                    free(FileAddress);
                return ret;
            }
            break;

        case 7:
            system("cls");
            ret = PrintBoundImportTable_V2(FileAddress);
            if (ret != 0)
            {
                if (FileAddress != NULL)
                    free(FileAddress);
                return ret;
            }
            break;

        case 8:
            system("cls");
            ret = PrintResourceTable_V2(FileAddress);
            if (ret != 0)
            {
                if (FileAddress != NULL)
                    free(FileAddress);
                return ret;
            }
            break;

        default:
            //释放空间
            if (FileAddress != NULL)
            {
                free(FileAddress);
            }
            exit(0);
            break;
        }
        system("pause");
    }

    system("pause");
    return ret;
}

