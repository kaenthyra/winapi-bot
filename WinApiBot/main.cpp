#include <windows.h>
#include <cwchar>

void PrintLine(const wchar_t* text) {
	HANDLE hDescriptor = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written{};
	WriteConsoleW(hDescriptor, text, (DWORD)wcslen(text), &written, NULL);
	WriteConsoleW(hDescriptor, L"\n", 1, &written, NULL);
}

int wmain() {
	PrintLine(L"Привет, Мир!");
	return 0;
}