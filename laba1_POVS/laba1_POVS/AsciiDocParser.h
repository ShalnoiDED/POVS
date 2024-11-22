#ifndef DOC_H
#define DOC_H

#include <string>

class AsciiDocEditor {
public:
    AsciiDocEditor();

    std::string ReadFile(std::string filename);

    void PrintHeadings(std::string content_);

    void PrintParagraphs(std::string content_);

    void PrintBoldText(std::string content_);

    std::string ChangeBold(std::string content);

    ~AsciiDocEditor();

private:
    std::string content_;  // Содержимое файла
};



#endif