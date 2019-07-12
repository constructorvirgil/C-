#include "iostream"
#include "windows.h"
using namespace std;
#define PIPE_NAME L"\\\\.\\Pipe\\test" 
int main()
{
	char buffer[1024];
	DWORD ReadNum;
	
	HANDLE hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 0, 0, 1000, NULL);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		cout << "创建命名管道失败！" << endl;
		CloseHandle(hPipe);
		return 0;
	}


	//阻塞
	//必须保证阻塞在这里的时候，客户端才WaitNamedPipe,CreateFile
	if (ConnectNamedPipe(hPipe, NULL) == FALSE)
	{
		cout << "与客户机连接失败！" << endl;
		CloseHandle(hPipe);
		return 0;
	}
	cout << "与客户机连接成功！" << endl;

	while (1)
	{
		//等待客户端发送信息，如果没有收到，则会阻塞在这里
		if (ReadFile(hPipe, buffer, 1024, &ReadNum, NULL) == FALSE)
		{
			cout << "读取数据失败！" << endl;
			break;
		}

		buffer[ReadNum] = 0;
		cout << "读取数据:" << buffer << endl;
	}

	cout << "关闭管道！" << endl;
	CloseHandle(hPipe);
	system("pause");
	return 0;
}
