#include <iostream>
#include <fstream>
#include <regex>
#include <locale>
#include "AsciiDocParser.h"

AsciiDocEditor::AsciiDocEditor()
{
}

AsciiDocEditor::~AsciiDocEditor()
{
}

std::string AsciiDocEditor::ReadFile(std::string filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Ошибка открытия файла!" << std::endl;
		return "";
	}

	content_ = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return content_;
}

std::string to_lower(const std::string& str) {
	std::string lower_str = str;
	std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
	return lower_str;
}


std::string process_match(const std::smatch& match) {
	return "*" + to_lower(match.str(1)) + "*";
}


void AsciiDocEditor::PrintHeadings(std::string content)
{
	std::regex headerRegex("^={1,6}\\s*(.+)$");

	std::cout << "Заголовки:" << std::endl;
	auto headersBegin = std::sregex_iterator(content.begin(), content.end(), headerRegex);
	auto headersEnd = std::sregex_iterator();

	for (std::sregex_iterator i = headersBegin; i != headersEnd; ++i) {
		std::cout << (*i)[1].str() << std::endl;
	}
}


void AsciiDocEditor::PrintParagraphs(std::string content)
{
	std::regex paragraphRegex("(^[^=\\s].+?(?:\\n{2,}|$))");
	std::cout << "\nАбзацы:" << std::endl;
	auto paragraphsBegin = std::sregex_iterator(content.begin(), content.end(), paragraphRegex);
	auto paragraphsEnd = std::sregex_iterator();
	for (std::sregex_iterator i = paragraphsBegin; i != paragraphsEnd; ++i) {
		std::cout << (*i)[1].str() << std::endl;
	}
}

void AsciiDocEditor::PrintBoldText(std::string content)
{
	int count = 0;
	std::regex boldRegex("\\*(.*?)\\*");
	std::cout << "\nЖирный текст:" << std::endl;
	auto boldsBegin = std::sregex_iterator(content.begin(), content.end(), boldRegex);
	auto boldsEnd = std::sregex_iterator();

	for (std::sregex_iterator i = boldsBegin; i != boldsEnd; ++i) {
		std::cout << (*i)[1].str() << std::endl;
		++count;
	}

	std::cout << "\nЖирный текст встречается: " << count << " раз" << std::endl;
}

std::string AsciiDocEditor::ChangeBold(std::string content)
{
	std::regex bold_in_headers_regex("(=+.*?)(\\*(.*?)\\*)");
	std::string result;

	auto words_begin = std::sregex_iterator(content.begin(), content.end(), bold_in_headers_regex);
	auto words_end = std::sregex_iterator();
	std::size_t last_pos = 0;

	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		std::smatch match = *i;
		result += content.substr(last_pos, match.position() - last_pos);
		result += match.str(1);
		std::string bold_text = match.str(3);
		std::transform(bold_text.begin(), bold_text.end(), bold_text.begin(), ::tolower);
		result += "*" + bold_text + "*";
		last_pos = match.position() + match.length();
	}
	result += content.substr(last_pos);

	return result;
}