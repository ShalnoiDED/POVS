// библиотека класса, чтоб его импортировать(неявно)
// файл dll_class.lib необходимо прописать компоновщику в main, чтобы на стадии компоновки загрузить библиотеку
// как прописать:
// выбираем проект -> правая кнопка мыши -> свойства -> Компоновщик -> Ввод -> Дополнительные зависимости
#include "..\dll_class\Header.h"

#include <locale>
#include <conio.h>
// библиотека windows для явной загрузки библиотеки
#include <windows.h>

int main() {
		setlocale(LC_ALL, "Russian");
		std::string filename;

		// с помощью винды загружаем библиотеку(явно)
		HINSTANCE hDLL = LoadLibraryA("dll_file.dll");
		if (!hDLL) {
			std::cout << "Не удалось открыть библиотеку";
			return 1;
		}

		// импортируем функцию из библиотеки по имени функции
		std::string(*iReadFile)(const std::string&) = (std::string(*)(const std::string&))GetProcAddress(hDLL, "ReadFile");
		DWORD SDAd = GetLastError();
		if (!ReadFile) {
			std::cout << "Не удалось импортировать функцию из библиотеки";
			return 1;
		}

		// читаем файл + обрабатываем исключения
		std::string content;
		while (1) {
			std::cout << "Введите имя файла: ";
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

		// объявляем класс и вызываем конструктор
		AsciiDocEditor asciiDocEditor(content);

		// выводим инфу
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