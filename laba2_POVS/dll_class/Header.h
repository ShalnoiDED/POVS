#pragma once

// ����� �������� �������, �������� �� EXPORT � �������������
// �� �������� ������ � ������� ����������, ������� ����� ������ ������� � ���� ���� ���� - ����� ����� �������������
// ��� ���������:
// �������� ������ -> ������ ������ ���� -> �������� -> C/C++ -> ������������ -> ����������� �������������
#ifdef EXPORT
#define DLL __declspec(dllexport)
// ����� �������� ������ ������� � ���� ���� ���� - ����� ����� ������������
#else
#define DLL __declspec(dllimport)
#endif

#include <iostream>
#include <fstream>
#include <regex>
#include <locale>
#include <string>

class DLL AsciiDocEditor {
    std::string content;
public:
    AsciiDocEditor();
    AsciiDocEditor(const std::string& content);
    ~AsciiDocEditor();
    void PrintHeadings();
    void PrintParagraphs();
    void PrintBoldText();
};
