#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
using str = std::string;
#if defined(_WIN32) || defined(_WIN64)  // Check for Windows
#define clear "cls"
    #define os "nt"
#else
#define clear "clear"
#define os "posix"
#endif
str wavetitled(const str &filename) {
    str titled = "              wave (" + filename + ")\n";
    return titled;
}

str saveFile(const str &path, const str &data) {
    std::ofstream outputFile; outputFile.open(path);
    if (outputFile.is_open()) {
        outputFile << data;
        outputFile.close();
        return "WRITE_SUCCESS";
    } else {
        return "WRITE_ERROR";
    }
}

str readFile(const str &path) {
    std::ifstream inputFile; inputFile.open(path);
    if (inputFile.is_open()) {
        str line;
        str temp;
        while (std::getline(inputFile, line)) {
            temp += line + '\n';
        }
        return temp;
    }
}

int main() {
    str mode;
    std::cout << "Modes: write, read\n";
    std::cout << "Enter mode.. ";
    std::getline(std::cin, mode);
    if (mode == "write") { mode = "write"; }
    else if (mode == "read") { mode = "read"; }
    else { std::cout << "Invalid mode.\n"; return 1;}
    str filename;
    std::cout << "Enter full file path: ";
    std::getline(std::cin, filename);
    if (std::filesystem::exists(filename)) {
        std::system(clear);

        if (mode == "write") { std::cout << wavetitled(filename); }
        else if (mode == "read" && filename == "/dev/urandom") { std::cout << "read error\n"; return 1; }
        else { std::cout << "              wave - ReadMode (" << filename << ")\n"; }
        bool terunning = true;
        str ui;
        int ln_num = 1;
        while (terunning) {
            if (mode == "write") {
                str temp;
                std::cout << ln_num << ' ';
                std::getline(std::cin, temp);
                if (temp == ".s") {
                    std::cout << "Saving...\n";
                    if (saveFile(filename, ui) == "WRITE_SUCCESS") {
                        continue;
                    } else {
                        std::cout << "Write error.\n";
                        return 1;
                    }
                } else if (temp == ".qs") {
                    std::cout << "Saving and exiting...\n";
                    terunning = false;
                    if (saveFile(filename, ui) == "WRITE_SUCCESS") {
                        continue;
                    } else {
                        std::cout << "Write error.\n";
                        return 1;
                    }
                } else if (temp == ".q") {
                    std::cout << "Exiting without saving...\n";
                    terunning = false;
                } else {
                    ln_num = ln_num + 1;
                    ui += temp + "\n";
                }
            } else if (mode == "read") {
                str data = readFile(filename);
                std::cout << data << '\n';
                std::cout << "Exiting..\n";
                return 0;
            } else {
                std::cout << "Invalid.\n";
                return 1;
            }
        }

    } else { // If specified path does not exist...
        creation:
            std::cout << "Specified path does not exist.\n";
            std::cout << "Would you like me to create it? (y/n) ";
            char creation_choice;
            std::cin >> creation_choice;
            if (creation_choice == 'y') {
                std::ofstream file;
                file.open(filename);
                if (file.is_open()) { file.close(); std::cout << "Created file.\n"; return 0; }
                else { std::cout << "Could not create file.\n"; return 1; }
            }
        return 0;
    }
    return 0;
}