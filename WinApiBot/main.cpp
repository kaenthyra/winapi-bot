#include <windows.h>
#include <cwchar>

bool PrintLine(const wchar_t* text) {
	HANDLE hDescriptor = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hDescriptor == INVALID_HANDLE_VALUE || hDescriptor == NULL) return false;
	DWORD written{};
	if (!WriteConsoleW(hDescriptor, text, (DWORD)wcslen(text), &written, NULL)) return false;
	if (!WriteConsoleW(hDescriptor, L"\n", 1, &written, NULL)) return false;
	return true;
}

int wmain() {
	if (!PrintLine(L"Привет, Мир!")) return 1;
	return 0;
}