#if !defined(JSON_PARSER)
#define JSON_PARSER
#include <any>
#include <string>
#include <map>
#include <vector>
#include <unordered_map>

enum ERROR_STATUS {
    FILE_NOT_FOUND = 1,
    FILE_ERROR_READ,
    FILE_READ_SUCCESS,
    FILE_PARSE_SUCCESS,
    ERROR = 84,
    SUCCESS = 0,
};

const std::unordered_map<ERROR_STATUS, std::string> error_code = {
        {FILE_NOT_FOUND, "File not found"},
        {FILE_ERROR_READ, "Error while reading file"},
        {FILE_READ_SUCCESS, "File read successfully"},
        {FILE_PARSE_SUCCESS, "File parsed successfully"},
        {ERROR, "Error"},
        {SUCCESS, "Success"}
};

std::map<std::string, std::any> parseJsonObj(std::string &json, ERROR_STATUS &error);
int cleanString(std::string &str);
std::string readFile(std::string &filename, ERROR_STATUS &error);
std::map<std::string, std::any> parser(std::string &filename, ERROR_STATUS &error);
#endif // JSON_PARSER
