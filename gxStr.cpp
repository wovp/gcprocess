#include "gxStr.h"
gxStr::gxStr() {
    // 构造函数的实现
}

gxStr::~gxStr() {
    // 析构函数的实现
}

int gxStr::extractID(const std::string& text) {
    // 找到 "Process ID:" 的位置
    size_t pos = text.find("Process ID:");
    if (pos != std::string::npos) {
        // 找到了 "Process ID:"，继续从该位置向后搜索数字
        size_t start = pos + strlen("Process ID:");
        size_t end = text.find(",", start); // 查找逗号的位置作为结束位置
        if (end != std::string::npos) {
            // 找到了逗号，提取数字
            std::string idString = text.substr(start, end - start);
            // 将提取的数字转换为整数并返回
            return std::stoi(idString);
        }
    }
    // 如果未找到逗号或 "Process ID:"，返回一个错误值（例如 -1）
    return -1;
}

int gxStr::extractID(const std::wstring& text) {
    // 找到 "Process ID:" 的位置
    size_t pos = text.find(L"Process ID:");
    if (pos != std::wstring::npos) {
        // 找到了 "Process ID:"，继续从该位置向后搜索数字
        size_t start = pos + wcslen(L"Process ID:");
        size_t end = text.find(L",", start); // 查找逗号的位置作为结束位置
        if (end != std::wstring::npos) {
            // 找到了逗号，提取数字
            std::wstring idString = text.substr(start, end - start);
            // 将提取的数字转换为整数并返回
            return std::stoi(idString);
        }
    }
    // 如果未找到逗号或 "Process ID:"，返回一个错误值（例如 -1）
    return -1;
}