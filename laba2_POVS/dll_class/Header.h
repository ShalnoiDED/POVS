#pragma once

// такая проверка сверяет, прописан ли EXPORT в препроцессоре
// он прописан только в проекте библиотеки, поэтому когда проект включит в себя этот файл - класс будет экспортирован
// как прописать:
// выбираем проект -> правая кнопка мыши -> свойства -> C/C++ -> Препроцессор -> Определения препроцессора
#ifdef EXPORT
#define DLL __declspec(dllexport)
// когда основной проект включит в себя этот файл - класс будет импортирован
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
