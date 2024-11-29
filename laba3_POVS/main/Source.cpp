// ���������� ������, ���� ��� �������������(������)
// ���� dll_class.lib ���������� ��������� ������������ � main, ����� �� ������ ���������� ��������� ����������
// ��� ���������:
// �������� ������ -> ������ ������ ���� -> �������� -> ����������� -> ���� -> �������������� �����������
#include "..\dll_class\Header.h"

#include <locale>
#include <conio.h>
#include <stdlib.h>
// ���������� ������� ��� ����� �������� ����������
#include <windows.h>
// ���������� ��� �������� �������
#include <thread>
// ���������� ��� �������� ��������
#include <mutex>

// ������� ��� ������������� ���������� ���������
std::mutex gLock;
// ������ �����, ���������� �����
std::vector<std::string> files;

// ��������� ������� ������ �� �����
std::string(*iReadFile)(const std::string&);
// ������� ������ ������ � ��������� ����������
void CheckFile(const WIN32_FIND_DATAA& file_data, const std::string& directory);
// ������� ���������� ���������
void PrintThing(const int& i);

bool compare(const std::string& left, const std::string& right)
{
	return left.length() > right.length();
}

int main() {
	// ����������
	std::string directory = "C:\\VSE_OSTALNOE\\Ycheba\\5 �������\\�� ��\\laba3_POVS\\main";
	setlocale(LC_ALL, "Russian");

	// � ������� ����� ��������� ����������(����)
	HINSTANCE hDLL = LoadLibraryA("dll_file.dll");
	if (!hDLL) {
		std::cout << "�� ������� ������� ����������";
		return 1;
	}

	// ����������� ������� �� ���������� �� ����� �������
	iReadFile = (std::string(*)(const std::string &))GetProcAddress(hDLL, "ReadFile");
	if (!ReadFile) {
		std::cout << "�� ������� ������������� ������� �� ����������";
		return 1;
	}

	// ������ �������
	std::vector<std::thread> threads;
	// ���������� �������
	HANDLE dir;
	// ��������� ��� �������� ������� ������
	WIN32_FIND_DATAA data{};

	// ���� ���������� �� ���� directory
	if ((dir = FindFirstFileA((directory + "/*").c_str(), &data)) == INVALID_HANDLE_VALUE) {
		std::cout << "�� ������� ������� ����������";
		return 1;
	}
	
	// ������ ���� ������������� � ��������� ������, ����� ��������� ������� CheckFile(data, directory, files)
	// ��� ����������� ������� ������������ ������ ������� {}
	do {
		threads.push_back(std::thread{ CheckFile, data, directory });
	} while (FindNextFileA(dir, &data));

	// � ����� ���� ���������� ���� �������. Join() ���������������� ����, ���� ��������� ����� �� ����������
	for (int i = 0; i < threads.size(); ++i)
		threads[i].join();

	// ��������� ���������� �������
	FindClose(dir);

	std::cout << "�������� ������� ���������� ������:\n"
		"1. �� �����������\n"
		"2. �� ��������\n";
	int choice;
	std::cin >> choice;

	if (choice == 1) {
		std::sort(files.begin(), files.end());
	}
	else if (choice == 2) {
		std::sort(files.begin(), files.end(), [](const std::string& a, const std::string& b) {
			return a > b;
			});
	}
	else {
		std::cout << "������������ �����, ����� �� ����� �������������.\n";
	}

	// ������� ������
	threads.clear();

	// ��������� ������ ������ ������
	for (int i = 0; i < files.size(); ++i) {
		threads.push_back(std::thread{ PrintThing, i });
		threads[i].join();
		}
	
	// � ����� ���� ���������� ���� �������
	//for (int i = 0; i < threads.size(); ++i)
	//	threads[i].join();

	std::cout << "���������� ��������� ��������, ������� ����� ������� ��� ������...";
	_getch();

	return 0;
}

void CheckFile(const WIN32_FIND_DATAA& file_data, const std::string& directory) {
	const std::string file_name = file_data.cFileName;
	const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

	if (file_name[0] == '.' || is_directory || file_name.find(".adoc") == std::string::npos)
		return;
	else
		files.push_back(directory + "\\" + file_name);
}

void PrintThing(const int& i) {
	// ��������� �������, ��� ����� ��� ��������� ������ ����� ����� �������������
	gLock.lock();
	// ���������� �����
	std::string content;
	std::cout << files[i] << "\n";

	// ��������� ���������� �� �����
	try {
		content = iReadFile(files[i]);
	}
	catch (const char* t) {
		std::cout << t << "\n";
		// ������������ �������
		gLock.unlock();
		return;
	}

	// ��������� ����� � �������� �����������
	AsciiDocEditor asciiDocEditor(content);

	// ������ ��������
	try {
		asciiDocEditor.PrintHeadings();
	}
	catch (const char* t) {
		std::cout << t << "\n";
	}
	try {
		asciiDocEditor.PrintParagraphs();
	}
	catch (const char* t) {
		std::cout << t << "\n";
	}
	try {
		asciiDocEditor.PrintBoldText();
	}
	catch (const char* t) {
		std::cout << t << "\n";
	}

	std::cout << "-------------------------------------------------------------------------------------------------\n";
	// ������������ �������
	gLock.unlock();
	return;
}