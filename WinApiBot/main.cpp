#include <windows.h>
#include <cwchar>
#include <cstring>

bool PrintLine(const wchar_t* text) {
	HANDLE hDescriptor = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hDescriptor == INVALID_HANDLE_VALUE || hDescriptor == NULL) {
		return false;
	}
	DWORD written{};
	if (!WriteConsoleW(hDescriptor, text, (DWORD)wcslen(text), &written, NULL)) {
		return false;
	}
	if (!WriteConsoleW(hDescriptor, L"\n", 1, &written, NULL)) {
		return false;
	}
	return true;
}

int wmain() {
	wchar_t exePath[MAX_PATH];
	if (!GetModuleFileNameW(NULL, exePath, MAX_PATH)) {
		return 1;
	}
	wchar_t* lastSlash = wcsrchr(exePath, L'\\');
	if (!lastSlash) {
		return 1;
	}
	lastSlash[1] = L'\0';
	wcscat_s(exePath, MAX_PATH, L"token.txt");
	HANDLE hTokenFile = CreateFileW(exePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hTokenFile == INVALID_HANDLE_VALUE) {
		PrintLine(L"Не удалось открыть token.txt");
		return 1;
	}
	PrintLine(L"Файл token.txt открыт");
	char buffer[256]{};
	DWORD written{};
	BOOL isRead = ReadFile(hTokenFile, buffer, 255, &written, NULL);
	CloseHandle(hTokenFile);
	if (!isRead) {
		PrintLine(L"Не удалось прочитать токен");
		return 1;
	}
	while (written > 0 && (buffer[written - 1] == '\r' || buffer[written - 1] == '\n' || buffer[written - 1] == '\t' || buffer[written - 1] == ' ')) {
		--written;
	}
	if (written == 0) {
		PrintLine(L"token.txt пустой");
		return 1;
	}
	buffer[written] = '\0';
	char* colon = strchr(buffer, ':');
	if (!colon) {
		PrintLine(L"Неверный формат токена, нет двоеточия");
		return 1;
	}
	int colonIndex = colon - buffer;
	if (colonIndex == 0) {
		PrintLine(L"Неверный формат токена, перед двоеточием пусто");
		return 1;
	}
	for (int i = 0; i < colonIndex; ++i) {
		if (!(buffer[i] >= '0' && buffer[i] <= '9')) {
			PrintLine(L"Неверный формат токена, ID бота должен состоять только из цифр");
			return 1;
		}
	}
	PrintLine(L"Токен прочитан");
	return 0;
}
