#include <iostream>
#include <fstream>
#include <string>

void replaceInFile(const std::string& filePath, const std::string& toReplace, const std::string& replaceWith) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open the file for reading!" << std::endl;
        return;
    }

    std::string fileContent;
    std::string line;

    while (std::getline(file, line)) {
        fileContent += line + "\n";
    }
    file.close();

    size_t pos = 0;
    while ((pos = fileContent.find(toReplace, pos)) != std::string::npos) {
        fileContent.replace(pos, toReplace.length(), replaceWith);
        pos += replaceWith.length();
    }

    // Write the new content back to the file
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Could not open the file for writing!" << std::endl;
        return;
    }
    outFile << fileContent;
    outFile.close();

    std::cout << "Replacement completed successfully." << std::endl;
}

int main() {
    std::string filePath;
    std::string toReplace;
    std::string replaceWith;

    std::cout << "Enter the file path: ";
    std::getline(std::cin, filePath);
    std::cout << "Enter the word to replace: ";
    std::getline(std::cin, toReplace);
    std::cout << "Enter the word to replace with: ";
    std::getline(std::cin, replaceWith);

    replaceInFile(filePath, toReplace, replaceWith);

    return 0;
}
