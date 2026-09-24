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

bool IsSecretChar(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '-') || (c == '_');
}

bool LoadToken(char* token, int tokenSize) {
	wchar_t exePath[MAX_PATH];
	if (!GetModuleFileNameW(NULL, exePath, MAX_PATH)) {
		return false;
	}
	wchar_t* lastSlash = wcsrchr(exePath, L'\\');
	if (!lastSlash) {
		return false;
	}
	lastSlash[1] = L'\0';
	wcscat_s(exePath, MAX_PATH, L"token.txt");
	HANDLE hTokenFile = CreateFileW(exePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hTokenFile == INVALID_HANDLE_VALUE) {
		PrintLine(L"Не удалось открыть token.txt");
		return false;
	}
	PrintLine(L"Файл token.txt открыт");
	DWORD written{};
	BOOL isRead = ReadFile(hTokenFile, token, tokenSize - 1, &written, NULL);
	CloseHandle(hTokenFile);
	if (!isRead) {
		PrintLine(L"Не удалось прочитать токен");
		return false;
	}
	while (written > 0 && (token[written - 1] == '\r' || token[written - 1] == '\n' || token[written - 1] == '\t' || token[written - 1] == ' ')) {
		--written;
	}
	if (written == 0) {
		PrintLine(L"token.txt пустой");
		return false;
	}
	token[written] = '\0';
	char* colon = strchr(token, ':');
	if (!colon) {
		PrintLine(L"Неверный формат токена, нет двоеточия");
		return false;
	}
	int colonIndex = colon - token;
	if (colonIndex == 0) {
		PrintLine(L"Неверный формат токена, перед двоеточием пусто");
		return false;
	}
	for (int i = 0; i < colonIndex; ++i) {
		if (!(token[i] >= '0' && token[i] <= '9')) {
			PrintLine(L"Неверный формат токена, ID бота должен состоять только из цифр");
			return false;
		}
	}
	int secretLength = written - colonIndex - 1;
	if (secretLength < 30) {
		PrintLine(L"Неверная длина токена");
		return false;
	}
	for (DWORD i = (colonIndex + 1); i < written; ++i) {
		// Ложная тревога анализатора кода
		// Этот цикл не выходит за written
		#pragma warning(suppress: 6385)
		if (!IsSecretChar(token[i])) {
			PrintLine(L"Неверная секретная часть токена");
			return false;
		}
	}
	return true;
}

int wmain() {
	char token[256]{};
	if (!LoadToken(token, sizeof(token))) {
		return 1;
	}
	PrintLine(L"Токен прочитан"); 
	return 0;
}
