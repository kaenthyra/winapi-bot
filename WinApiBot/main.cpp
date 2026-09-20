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
	wchar_t exePath[MAX_PATH];
	if (!GetModuleFileNameW(NULL, exePath, MAX_PATH)) return 1;
	wchar_t* lastSlash = wcsrchr(exePath, L'\\');
	if (!lastSlash) return 1;
	lastSlash[1] = L'\0';
	wcscat_s(exePath, MAX_PATH, L"token.txt");
	PrintLine(exePath);
	HANDLE hTokenFile = CreateFileW(exePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hTokenFile == INVALID_HANDLE_VALUE) {
		PrintLine(L"Не удалось открыть token.txt");
		return 1;
	}
	PrintLine(L"Файл token.txt открыт");
	CloseHandle(hTokenFile);
	return 0;
}
