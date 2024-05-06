#include "gxStr.h"
gxStr::gxStr() {
    // ���캯����ʵ��
}

gxStr::~gxStr() {
    // ����������ʵ��
}

int gxStr::extractID(const std::string& text) {
    // �ҵ� "Process ID:" ��λ��
    size_t pos = text.find("Process ID:");
    if (pos != std::string::npos) {
        // �ҵ��� "Process ID:"�������Ӹ�λ�������������
        size_t start = pos + strlen("Process ID:");
        size_t end = text.find(",", start); // ���Ҷ��ŵ�λ����Ϊ����λ��
        if (end != std::string::npos) {
            // �ҵ��˶��ţ���ȡ����
            std::string idString = text.substr(start, end - start);
            // ����ȡ������ת��Ϊ����������
            return std::stoi(idString);
        }
    }
    // ���δ�ҵ����Ż� "Process ID:"������һ������ֵ������ -1��
    return -1;
}

int gxStr::extractID(const std::wstring& text) {
    // �ҵ� "Process ID:" ��λ��
    size_t pos = text.find(L"Process ID:");
    if (pos != std::wstring::npos) {
        // �ҵ��� "Process ID:"�������Ӹ�λ�������������
        size_t start = pos + wcslen(L"Process ID:");
        size_t end = text.find(L",", start); // ���Ҷ��ŵ�λ����Ϊ����λ��
        if (end != std::wstring::npos) {
            // �ҵ��˶��ţ���ȡ����
            std::wstring idString = text.substr(start, end - start);
            // ����ȡ������ת��Ϊ����������
            return std::stoi(idString);
        }
    }
    // ���δ�ҵ����Ż� "Process ID:"������һ������ֵ������ -1��
    return -1;
}