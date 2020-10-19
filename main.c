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


    printf("������EXE�ļ���·����");
    scanf("%s", &FilePath);

    //1�����ļ����뵽�ڴ�
    ret = MyReadFile_V2(&FileAddress, FilePath);
    if (ret != 0)
    {
        if (FileAddress != NULL)
            free(FileAddress);
        return ret;
    }

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)FileAddress;
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + (DWORD)pDosHeader->e_lfanew);

    //2���ж��Ƿ���EXE�ļ�
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE || pNtHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        ret = -99;
        printf("���ļ�����EXE�ļ�!\n");
        return ret;
    }

    //3��ѡ�����������
    while (1)
    {
        system("cls");
        printf("===================================================\n");
        printf("|                ��ӭʹ��С�Ƶ�EXE������            \n");
        printf("|--------------------------------------------------\n");
        printf("|      1.����EXE�ļ�ͷ                               \n");
        printf("|      2.����EXE�ڱ�                                 \n");
        printf("|      3.����EXEĿ¼                                 \n");
        printf("|      4.����EXE������                               \n");
        printf("|      5.����EXE�ض�λ��                             \n");
        printf("|      6.����EXE�����                               \n");
        printf("|      7.����EXE�󶨵����                            \n");
        printf("|      8.����EXE��Դ��                               \n");
        printf("|--------------------------------------------------\n");
        printf("|      �������������˳�����                          \n");
        printf("===================================================\n");
        printf("��ѡ����Ҫ���������ݣ�");
        scanf("%d", &Choose);

        switch (Choose)
        {
        case 1:
            system("cls");
            ret = PrintEXEDosHeader_V2(FileAddress);//DOSͷ
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
            //�ͷſռ�
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

