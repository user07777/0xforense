#include <windows.h>

#include <iostream>

#include <string>

void RecentOpen() {
	HKEY RecentDocs, extension;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs", 0, KEY_READ, &RecentDocs) == ERROR_SUCCESS) {
		char key[256];
		DWORD keysz;
		DWORD i = 0;
		std::string ext;
		while (true) {
			keysz = 256;
			LONG res = RegEnumKeyExA(RecentDocs, i, key, &keysz, 0, 0, 0, 0);
			if (res == ERROR_NO_MORE_ITEMS) break;
			else if (res == ERROR_SUCCESS) std::cout << key << '\n';
			else {
				std::cerr << "RegEnumKeyExA 1: " << GetLastError() << '\n';
				break;
			}
			i++;
		}
		std::cout << "\nSelecione uma extensao: ";
		std::cin >> ext;
		if (RegOpenKeyExA(RecentDocs, ext.c_str(), 0, KEY_READ, &extension) == ERROR_SUCCESS) {
			char val[256];
			unsigned char buff[2048];
			DWORD valSz, buffSz, type;
			i = 0;
			std::cout << "\nfound:\n";
			while (true) {
				valSz = 256;
				buffSz = 2048;
				LONG res = RegEnumValueA(extension, i, val, &valSz, 0, &type, buff, &buffSz);
				if (res == ERROR_NO_MORE_ITEMS) break;
				else if (res == ERROR_SUCCESS) {
					wchar_t* wide = (wchar_t*)buff;
					int len = (buffSz / sizeof(wchar_t));
					bool found = false;
					for (int j = 0; j < len - 1; ++j) {
						if (iswprint(wide[j]) && iswprint(wide[j + 1])) {
							std::wcout << (wchar_t*)&wide[j] << std::endl;
							found = true;
							break;
						}
					}
				}
				else {
					std::cerr << "RegEnumValueA 2: " << GetLastError() << '\n';
					break;
				}
				++i;
			}
			RegCloseKey(extension);
		}
 
		RegCloseKey(RecentDocs);
	}
 
 }
int main() {
	RecentOpen();
}
