// SetFileTime.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	FILETIME ft,lft;
	SYSTEMTIME st;
	BOOL f;

	if(argc<5){
		std::cout<<"error"<<"Usage: SetFileTime *.* year month day"<<std::endl;
		return 0;
	}

	st.wYear=atoi(argv[2]);
	st.wMonth=atoi(argv[3]);
	st.wDay=atoi(argv[4]);
	st.wHour=0;
	st.wMinute=0;
	st.wSecond=0;
	st.wMilliseconds=0;
	st.wDayOfWeek=0;

	std::cout<<"Year: "<<st.wYear<<"\tMonth: "<<st.wMonth<<"\tDay: "<<st.wDay<<std::endl;
	SystemTimeToFileTime(&st,&lft);
	LocalFileTimeToFileTime(&lft,&ft);

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	std::cout<<"Target file is "<<argv[1]<<std::endl;
	hFind = FindFirstFile(argv[1], &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		printf ("Invalid File Handle. GetLastError reports %d\n", GetLastError ());
		return (0);
	} 
	else 
	{
		do{
			HANDLE h=CreateFile(FindFileData.cFileName,FILE_WRITE_ATTRIBUTES,NULL,NULL,OPEN_EXISTING,NULL,NULL);
			if (h== INVALID_HANDLE_VALUE) 
			{ 
				std::cout<<"Could not open file:"<<FindFileData.cFileName<<' ';   // process error 

				{
					DWORD MsgID = GetLastError();
					char *errorMessage;
					std::cerr << "Error has occured: ";

					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, MsgID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(LPTSTR) &errorMessage, 0, 0);

					std::cout << errorMessage << "\n";

					LocalFree(errorMessage);
				}

			}else{
				f=SetFileTime(h,&ft,&ft,&ft);
				if (!f)
				{
					std::cout<<"Failed to prcess";
				}

				std::cout<<FindFileData.cFileName<<std::endl;

				CloseHandle(h);
			}
		}while(FindNextFile(hFind,&FindFileData));
		FindClose(hFind);
	}

	return 0;
}

