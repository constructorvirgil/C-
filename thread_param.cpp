#include <windows.h>
#include <strsafe.h>
//win2003SDK必须安装　要不无此头文件。此文件是为了实现StringCchPrintf，StringCchLength。
#define MAX_THREADS    3
#define BUF_SIZE    255
typedef struct _MyData {
	int val1;
	int val2;
}MYDATA, *PMYDATA;
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	HANDLE hStdout;
	PMYDATA pData;
	TCHAR msgBuf[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;
	//Cast the parameter to the correct data type.
	//把参数转换成正确的数据类型
	pData = (PMYDATA)lpParam;
	//Print the parameter values using thread-safe functions.
	//线程安全地打印参数的值
	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters=%d,%d\n"),
		pData->val1, pData->val2);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuf, cchStringSize, &dwChars, NULL);
	//Free the memory allocated by the caller for the thread
	//data structure.
	HeapFree(GetProcessHeap(), 0, pData);
	return 0;
}
void main()
{
	PMYDATA pData;
	DWORD dwThreadId[MAX_THREADS];//保存所有线程ID
	HANDLE hThread[MAX_THREADS];//用来保存所有线程的句柄
	int i;
	//Create MAX_THREADS worker threads.
	for (i = 0; i < MAX_THREADS; i++)
	{
		//Allocate memory for thread data.
		pData = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(MYDATA));
		if (pData == NULL)
			ExitProcess(2);
		//Generate unique data for each thread.
		pData->val1 = i;
		pData->val2 = i + 100;
		hThread[i] = CreateThread(
			NULL,//default security attributes
			0,//use default stack size
			ThreadProc,//thread function
			pData,//argument to thread function
			0,//use default creation flags
			&dwThreadId[i]);//returns the thread identifier
			//Check there turn value for success.
		if (hThread[i] == NULL)
		{
			ExitProcess(i);
		}
	}
	//Wait until all threads have terminated.
	WaitForMultipleObjects(MAX_THREADS, hThread, TRUE, INFINITE);
	//Close all thread handle supon completion.
	for (i = 0; i < MAX_THREADS; i++)
	{
		CloseHandle(hThread[i]);
	}
}