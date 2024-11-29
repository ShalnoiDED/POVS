// библиотека класса, чтоб его импортировать(неявно)
// файл dll_class.lib необходимо прописать компоновщику в main, чтобы на стадии компоновки загрузить библиотеку
// как прописать:
// выбираем проект -> правая кнопка мыши -> свойства -> Компоновщик -> Ввод -> Дополнительные зависимости
#include "..\dll_class\Header.h"

#include <locale>
#include <conio.h>
#include <stdlib.h>
// библиотека виндовс для явной загрузки библиотеки
#include <windows.h>
// библиотека для создания потоков
#include <thread>
// библиотека для создания мьютекса
#include <mutex>

// мьютекс для синхронизации извлечения элементов
std::mutex gLock;
// вектор строк, содержащих файлы
std::vector<std::string> files;

// объявляем функцию чтения из файла
std::string(*iReadFile)(const std::string&);
// функция поиска файлов в указанной директории
void CheckFile(const WIN32_FIND_DATAA& file_data, const std::string& directory);
// функция извлечения элементов
void PrintThing(const int& i);

bool compare(const std::string& left, const std::string& right)
{
	return left.length() > right.length();
}

int main() {
	// директория
	std::string directory = "C:\\VSE_OSTALNOE\\Ycheba\\5 семестр\\ПО ВС\\laba3_POVS\\main";
	setlocale(LC_ALL, "Russian");

	// с помощью винды загружаем библиотеку(явно)
	HINSTANCE hDLL = LoadLibraryA("dll_file.dll");
	if (!hDLL) {
		std::cout << "Не удалось открыть библиотеку";
		return 1;
	}

	// импортируем функцию из библиотеки по имени функции
	iReadFile = (std::string(*)(const std::string &))GetProcAddress(hDLL, "ReadFile");
	if (!ReadFile) {
		std::cout << "Не удалось импортировать функцию из библиотеки";
		return 1;
	}

	// вектор потоков
	std::vector<std::thread> threads;
	// дескриптор объекта
	HANDLE dir;
	// структура для хранения свойств файлов
	WIN32_FIND_DATAA data{};

	// ищем директорию по пути directory
	if ((dir = FindFirstFileA((directory + "/*").c_str(), &data)) == INVALID_HANDLE_VALUE) {
		std::cout << "Не удалось открыть директорию";
		return 1;
	}
	
	// каждый файл рассматриваем в отдельном потоке, поток выполняет функцию CheckFile(data, directory, files)
	// для определения потоков используются лямбда функция {}
	do {
		threads.push_back(std::thread{ CheckFile, data, directory });
	} while (FindNextFileA(dir, &data));

	// в цикле ждем завершение всех потоков. Join() приостанавливает цикл, пока указанный поток не завершится
	for (int i = 0; i < threads.size(); ++i)
		threads[i].join();

	// закрываем дескриптор объекта
	FindClose(dir);

	std::cout << "Выберите порядок сортировки файлов:\n"
		"1. По возрастанию\n"
		"2. По убыванию\n";
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
		std::cout << "Некорректный выбор, файлы не будут отсортированы.\n";
	}

	// очищаем вектор
	threads.clear();

	// запускаем потоки чтения файлов
	for (int i = 0; i < files.size(); ++i) {
		threads.push_back(std::thread{ PrintThing, i });
		threads[i].join();
		}
	
	// в цикле ждем завершение всех потоков
	//for (int i = 0; i < threads.size(); ++i)
	//	threads[i].join();

	std::cout << "Извлечение элементов окончено, нажмите любую клавишу для выхода...";
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
	// блокируем мьютекс, тем самым все остальные потоки будут ждать разблокировки
	gLock.lock();
	// содержание файла
	std::string content;
	std::cout << files[i] << "\n";

	// считываем содержимое из файла
	try {
		content = iReadFile(files[i]);
	}
	catch (const char* t) {
		std::cout << t << "\n";
		// Разблокируем мьютекс
		gLock.unlock();
		return;
	}

	// объявляем класс и вызываем конструктор
	AsciiDocEditor asciiDocEditor(content);

	// читаем элементы
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
	// Разблокируем мьютекс
	gLock.unlock();
	return;
}