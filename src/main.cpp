#include "WordFilter.h"
#include <iostream>

using namespace std;

int main()
{
	WordFilter* pWordFilter = WordFilter::sharedInstace();
	if (NULL == pWordFilter)
	{
		std::cout << "ERROR!!! pWordFilter == NULL" << endl;
		return 1;
	}

	pWordFilter->load("../../../../GitHub/SensitiveWords/config/SensitiveWords.txt");

	std::string tempStr = "asdfiasdѪ��Ͻ����¹�½����ƵĹ��˵��������ƨ��";
	pWordFilter->censor(tempStr);
	cout << "after filter tempStr = " << tempStr << endl;

	tempStr = "asdfiasdѪ��Ͻ����¹�½����ƵĹ��˵��������ƨ��";
	pWordFilter->censor(tempStr, "�Ǻ�");
	cout << "after filter tempStr = " << tempStr << endl;

	system("pause");
}