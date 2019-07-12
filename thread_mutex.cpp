#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <mutex>
using namespace std;

const uint8_t INTANCES = 3;
int _count = 0;
HANDLE	hThread[INTANCES];
DWORD	dwThreadId[INTANCES];
mutex mtx;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD id = GetCurrentThreadId();

	mtx.lock();
	for (int i = 0; i <3; i++) {
		cout << "In thread:"	<< id	<< endl;
		cout <<	"Count:"		<<_count++		<< endl;
		Sleep(100);
	}
	mtx.unlock();

	return 0;
}
int main()
{

	for (int i = 0; i < INTANCES; i++) {
		hThread [i] = CreateThread(
			NULL,
			0,
			ThreadProc,
			NULL,
			0,
			&dwThreadId[i]);

	}
	WaitForMultipleObjects(INTANCES, hThread, TRUE, INFINITE);

	for (int i = 0; i < INTANCES; i++) {
		CloseHandle(hThread[i]);
	}
	
	return 0;

}




