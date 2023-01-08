#include "parser.hpp"
#include <iostream>
#include <fstream>

std::map<std::string, std::any> parser(std::string &filename, ERROR_STATUS &error)
{
    std::string fileContent = "";
    std::map<std::string, std::any> content = {};

    fileContent = readFile(filename, error);
    if (error == ERROR_STATUS::FILE_READ_SUCCESS) {
        return parseJsonObj(fileContent, error);
    }
    return {};
}

std::string readFile(std::string &filename, ERROR_STATUS &error)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    const size_t file_size = file.tellg();
    char buffer[file_size + 1] = {'\0'};

    if (!file.is_open()) {
        error = ERROR_STATUS::FILE_NOT_FOUND;
    }
    if (file.bad()) {
        error = ERROR_STATUS::FILE_ERROR_READ;
    }
    file.seekg(0, std::ios::beg);
    if (!file.read(buffer, file_size)) {
        error = ERROR_STATUS::FILE_ERROR_READ;
    }
    if (error == ERROR_STATUS::FILE_ERROR_READ || error == ERROR_STATUS::FILE_NOT_FOUND) {
        std::cout << error_code.at(error) << std::endl;
        return "";
    }
    error = ERROR_STATUS::FILE_READ_SUCCESS;
    file.close();
    return buffer;
}

int cleanString(std::string &str)
{
    size_t pos = 0;

    while ((pos = str.find(' ')) != std::string::npos) {
        str.erase(pos, 1);
    }
    while ((pos = str.find('"')) != std::string::npos) {
        str.erase(pos, 1);
    }
    while ((pos = str.find('}')) != std::string::npos) {
        str.erase(pos, 1);
    }
    while ((pos = str.find('{')) != std::string::npos) {
        str.erase(pos, 1);
    }
    while ((pos = str.find('[')) != std::string::npos) {
        str.erase(pos, 1);
    }
    while ((pos = str.find(']')) != std::string::npos) {
        str.erase(pos, 1);
    }
    while ((pos = str.find(',')) != std::string::npos) {
        str.erase(pos, 1);
    }
    while ((pos = str.find('\n')) != std::string::npos) {
        str.erase(pos, 1);
    }
    return 0;
}

std::map<std::string, std::any> parseJsonObj(std::string &json, ERROR_STATUS &error)
{
    std::map<std::string, std::any> content = {};
    std::string key = "";
    std::string value = "";
    size_t pos = 0;

    while ((pos = json.find(':')) != std::string::npos) {
        key = json.substr(0, pos);
        cleanString(key);
        json.erase(0, pos + 1);
        pos = json.find(',');
        if (json[1] == '{') {
            content[key] = parseJsonObj(json, error);
            continue;
        } else {
            value = json.substr(0, pos);
            if (value.find('}') != std::string::npos) {
                cleanString(value);
                content[key] = value;
                json.erase(0, value.length() + 10);
                return content;
            } else {
                cleanString(value);
                json.erase(0, pos + 1);
                content[key] = value;
            }
        }
    }
    error = ERROR_STATUS::FILE_PARSE_SUCCESS;
    return content;
}

int printParsedMap(std::map<std::string, std::any> content)
{
    for (auto &it : content) {
        if (it.second.type() == typeid(std::string)) {
            std::cout << it.first << " : " << std::any_cast<std::string>(it.second) << std::endl;
        } else if (it.second.type() == typeid(std::map<std::string, std::any>)) {
            std::cout << it.first << " : " << std::endl;
            printParsedMap(std::any_cast<std::map<std::string, std::any>>(it.second));
        }
    }
    return 0;
}

int main(const int argc, const char *argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return ERROR_STATUS::ERROR;
    }
    ERROR_STATUS error = ERROR_STATUS::SUCCESS;
    std::string filename = argv[1];
    std::map<std::string, std::any> myData = parser(filename, error);
    if (error != ERROR_STATUS::FILE_PARSE_SUCCESS) {
        std::cout << error_code.at(error) << std::endl;
        return ERROR_STATUS::ERROR;
    }
    printParsedMap(myData);
    return ERROR_STATUS::SUCCESS;
}