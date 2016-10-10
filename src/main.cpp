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

	pWordFilter->load("config/SensitiveWords.txt");

	std::string tempStr = "你妈逼的过滤血书的婊子养的屁啊";
	pWordFilter->censor(tempStr);
	cout << "after filter tempStr = " << tempStr << endl;

	system("pause");
}