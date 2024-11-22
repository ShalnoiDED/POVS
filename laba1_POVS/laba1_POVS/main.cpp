#include "Tool.h"
#include "AsciiDocParser.h"
#include <locale>
#include <iostream>

int main() {
	setlocale(LC_ALL, "Russian");

	AsciiDocEditor asciiDocEditor;
	std::string filename;
	std::cout << "Enter file name: ";
	std::cin >> filename;
	std::string content = asciiDocEditor.ReadFile(filename);
	content = asciiDocEditor.ChangeBold(content);
	asciiDocEditor.PrintHeadings(content);
	asciiDocEditor.PrintParagraphs(content);
	asciiDocEditor.PrintBoldText(content);
	
	return 0;
}