#include "iostream"
#include "windows.h"

using namespace std;
#define PIPE_NAME L"\\\\.\\Pipe\\test"
int  main()
{
	char buffer[1024];
	DWORD WriteNum;

	//如果当时服务器没有打开，那么这个函数会直接返回失败
	if (WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER) == FALSE)
	{
		cout << "等待命名管道实例失败！" << endl;
		return 0;
	}

	//服务器准备好环境后，才打开这个管道
	HANDLE hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		cout << "创建命名管道失败！" << endl;
		CloseHandle(hPipe);
		return 0;
	}
	cout << "与服务器连接成功！" << endl;
	while (1)
	{
		cin >> buffer;
		//WriteNum返回写入的字节数
		if (WriteFile(hPipe, buffer, strlen(buffer), &WriteNum, NULL) == FALSE)
		{
			cout << "数据写入管道失败！" << endl;
			break;
		}
	}

	cout << "关闭管道！" << endl;
	CloseHandle(hPipe);
	system("pause");
	return 0;
}
