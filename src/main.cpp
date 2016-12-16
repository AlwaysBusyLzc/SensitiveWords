#include "SensitiveWordFilter.h"
#include <iostream>


using namespace std;

std::wstring Ascii2Unicode(const std::string& str)
{
	// 先计算转换后的大小
	int nLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	wchar_t* pUnicode = new wchar_t[nLen + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, nLen + 1);
	wstring ret_str = pUnicode;
	delete[]pUnicode;

	return ret_str;
}

string Unicode2Utf8(const std::wstring& wstr)
{
	int nLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pStr = new char[nLen + 1];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pStr, nLen + 1, nullptr, nullptr);
	string ret_str = pStr;
	delete[]pStr;
	return ret_str;
}

string Ascii2Utf8(const std::string& str)
{
	return Unicode2Utf8(Ascii2Unicode(str));
}

int main()
{
	CSensitiveWordFilter* pWordFilter = new CSensitiveWordFilter();
	if (NULL == pWordFilter)
	{
		std::cout << "ERROR!!! pWordFilter == NULL" << endl;
		return 1;
	}

	pWordFilter->load("../../../../GitHub/SensitiveWords/config/ChatSensitiveWord.txt");

	char szTest[64];

	while (true)
	{
		cin.getline(szTest, 64);
		//wstring wszTemp = Ascii2Unicode(string(szTest));

		std::string szUtf8 = Ascii2Utf8(string(szTest));
		//std::transform(szUtf8.begin(), szUtf8.end(), szUtf8.begin(), tolower);
		//cout << "after transform string is " << szUtf8 << endl;

		if (pWordFilter->censor(szUtf8))
		{
			cout << "check str has sensitiveWorld+++++++++++++++++++++++++++++++++++++++++++++++" << endl;
		}
	}

	system("pause");

	delete pWordFilter;
	return 0;
}
