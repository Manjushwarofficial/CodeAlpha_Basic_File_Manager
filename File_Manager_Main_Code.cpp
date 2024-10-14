#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

class FileManager {
private:
    fs::path currentPath;

    void listDirectory() {
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            std::cout << (entry.is_directory() ? "D " : "F ")
                      << entry.path().filename().string() << std::endl;
        }
    }

    void changeDirectory(const std::string& dir) {
        fs::path newPath = currentPath / dir;
        if (fs::exists(newPath) && fs::is_directory(newPath)) {
            currentPath = fs::canonical(newPath);
            std::cout << "Changed to: " << currentPath << std::endl;
        } else {
            std::cout << "Invalid directory." << std::endl;
        }
    }

    void viewFile(const std::string& filename) {
        fs::path filePath = currentPath / filename;
        if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
            std::ifstream file(filePath);
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << std::endl;
            }
        } else {
            std::cout << "File not found or not a regular file." << std::endl;
        }
    }

    void createDirectory(const std::string& dirname) {
        fs::path newDir = currentPath / dirname;
        if (fs::create_directory(newDir)) {
            std::cout << "Directory created: " << newDir << std::endl;
        } else {
            std::cout << "Failed to create directory." << std::endl;
        }
    }

    void copyFile(const std::string& source, const std::string& destination) {
        fs::path sourcePath = currentPath / source;
        fs::path destPath = currentPath / destination;
        try {
            fs::copy_file(sourcePath, destPath, fs::copy_options::overwrite_existing);
            std::cout << "File copied successfully." << std::endl;
        } catch (const fs::filesystem_error& e) {
            std::cout << "Error copying file: " << e.what() << std::endl;
        }
    }

    void moveFile(const std::string& source, const std::string& destination) {
        fs::path sourcePath = currentPath / source;
        fs::path destPath = currentPath / destination;
        try {
            fs::rename(sourcePath, destPath);
            std::cout << "File moved successfully." << std::endl;
        } catch (const fs::filesystem_error& e) {
            std::cout << "Error moving file: " << e.what() << std::endl;
        }
    }

public:
    FileManager() : currentPath(fs::current_path()) {}

    void run() {
        std::string command;
        while (true) {
            std::cout << currentPath.string() << "> ";
            std::getline(std::cin, command);
            std::vector<std::string> tokens;
            size_t pos = 0;
            while ((pos = command.find(' ')) != std::string::npos) {
                tokens.push_back(command.substr(0, pos));
                command.erase(0, pos + 1);
            }
            tokens.push_back(command);

            if (tokens[0] == "exit") break;
            else if (tokens[0] == "ls") listDirectory();
            else if (tokens[0] == "cd" && tokens.size() > 1) changeDirectory(tokens[1]);
            else if (tokens[0] == "view" && tokens.size() > 1) viewFile(tokens[1]);
            else if (tokens[0] == "mkdir" && tokens.size() > 1) createDirectory(tokens[1]);
            else if (tokens[0] == "cp" && tokens.size() > 2) copyFile(tokens[1], tokens[2]);
            else if (tokens[0] == "mv" && tokens.size() > 2) moveFile(tokens[1], tokens[2]);
            else std::cout << "Invalid command." << std::endl;
        }
    }
};

int main() {
    FileManager fm;
    fm.run();
    return 0;
}
