// ���������� ������, ���� ��� �������������(������)
// ���� dll_class.lib ���������� ��������� ������������ � main, ����� �� ������ ���������� ��������� ����������
// ��� ���������:
// �������� ������ -> ������ ������ ���� -> �������� -> ����������� -> ���� -> �������������� �����������
#include "..\dll_class\Header.h"

#include <locale>
#include <conio.h>
// ���������� windows ��� ����� �������� ����������
#include <windows.h>

int main() {
		setlocale(LC_ALL, "Russian");
		std::string filename;

		// � ������� ����� ��������� ����������(����)
		HINSTANCE hDLL = LoadLibraryA("dll_file.dll");
		if (!hDLL) {
			std::cout << "�� ������� ������� ����������";
			return 1;
		}

		// ����������� ������� �� ���������� �� ����� �������
		std::string(*iReadFile)(const std::string&) = (std::string(*)(const std::string&))GetProcAddress(hDLL, "ReadFile");
		DWORD SDAd = GetLastError();
		if (!ReadFile) {
			std::cout << "�� ������� ������������� ������� �� ����������";
			return 1;
		}

		// ������ ���� + ������������ ����������
		std::string content;
		while (1) {
			std::cout << "������� ��� �����: ";
			std::cin >> filename;
			try {
				content = iReadFile(filename);
			}
			catch (const char* t) {
				printf(t);
				continue;
			}
			break;
		}

		// ��������� ����� � �������� �����������
		AsciiDocEditor asciiDocEditor(content);

		// ������� ����
		try {
			asciiDocEditor.PrintHeadings();
		}
		catch (const char* t) {
			printf(t);
		}
		try {
			asciiDocEditor.PrintParagraphs();
		}
		catch (const char* t) {
			printf(t);
		}
		try {
			asciiDocEditor.PrintBoldText();
		}
		catch (const char* t) {
			printf(t);
		}

		_getch();

	return 0;
}