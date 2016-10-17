#include "SensitiveWordFilter.h"
#include <iostream>

using namespace std;

int main()
{
	CSensitiveWordFilter* pWordFilter = new CSensitiveWordFilter();
	if (NULL == pWordFilter)
	{
		std::cout << "ERROR!!! pWordFilter == NULL" << endl;
		return 1;
	}

	pWordFilter->load("../../../../GitHub/SensitiveWords/config/SensitiveWords.txt");

	std::string tempStr = "asdfiasd血书赶紧啊德国陆军妈逼的过滤的婊子养的屁啊";
	pWordFilter->censor(tempStr);
	cout << "after filter tempStr = " << tempStr << endl;

	tempStr = "asdfiasd血书赶紧啊德国陆军妈逼的过滤的婊子养的屁啊";
	pWordFilter->censor(tempStr, "呵呵");
	cout << "after filter tempStr = " << tempStr << endl;

	system("pause");

	delete pWordFilter;
	return 0;
}