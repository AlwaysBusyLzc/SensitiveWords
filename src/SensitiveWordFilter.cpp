#include "SensitiveWordFilter.h"
#include <iostream>
#include <fstream>

//#define PACE 1

wstring Utf82Unicode(const string& str)
{
	// 预算-缓冲区中宽字节的长度    
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	// 给指向缓冲区的指针变量分配内存    
	wchar_t *pUnicode = new wchar_t[unicodeLen + 1];
	// 开始向缓冲区转换字节    
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, unicodeLen + 1);
	wstring ret_str = pUnicode;
	delete[]pUnicode;
	return ret_str;
}

string Unicode2Ascii(const wstring& wstr)
{
	// 预算-缓冲区中多字节的长度    
	int ansiiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// 给指向缓冲区的指针变量分配内存    
	char *pAssii = new char[ansiiLen + 1];
	// 开始向缓冲区转换字节    
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, ansiiLen + 1, nullptr, nullptr);
	string ret_str = pAssii;
	delete[]pAssii;
	return ret_str;
}

string Utf82Ascii(const std::string &str)
{
	return Unicode2Ascii(Utf82Unicode(str));
}


// 获取utf8字符长度
size_t GetUtf8CharSize(BYTE byFirst)
{
	if (byFirst >= 0xFC)
	{
		return 6;
	}
	else if (byFirst >= 0xF8)
	{
		return 5;
	}
	else if (byFirst >= 0xF0)
	{
		return 4;
	}
	else if (byFirst >= 0xE0)
	{
		return 3;
	}
	else if (byFirst >= 0xC0)
	{
		return 2;
	}

	return 1;
}

WordNode::WordNode(string character)
{
	m_bIsFinish = false;
	m_parent = NULL;

	if (character.size() == 0)
	{
		m_character = "";
	}
	else
	{
		size_t nCharSize = GetUtf8CharSize((unsigned)character[0]);
		if (character.size() == nCharSize)
		{
			m_character.assign(character);
		}
		else
		{
			cout << "error" << endl;
		}
	}
}

WordNode* WordNode::findChild(string& nextCharacter)
{
	_TreeMapIterator TreeMapIt = m_map.find(nextCharacter);
	if (TreeMapIt == m_map.end())
	{
		return NULL;
	}
	else
	{
		return &TreeMapIt->second;
	}
}

WordNode* WordNode::insertChild(string& nextCharacter)
{
	if (!findChild(nextCharacter))
	{
		m_map.insert(pair<string,WordNode>(nextCharacter, WordNode(nextCharacter)));
		return &(m_map.find(nextCharacter)->second);
	}
	return NULL;
}


WordNode* WordTree::insert(string &keyword)
{
	return insert(&m_emptyRoot, keyword);
}

WordNode* WordTree::insert(const char* keyword)
{
	string wordstr(keyword);
	return insert(wordstr);
}

WordNode* WordTree::find(string& keyword)
{
	return find(&m_emptyRoot,keyword);
}

WordNode* WordTree::insert(WordNode* parent, string& keyword)
{
	if (keyword.size() == 0)
	{
		return NULL;
	}

	size_t nCharSize = GetUtf8CharSize((unsigned)keyword[0]);
	string firstChar = keyword.substr(0, nCharSize);
	WordNode* firstNode = parent->findChild(firstChar);
	if (firstNode == NULL)
	{
		return insertBranch(parent,keyword);
	}
	string restChar = keyword.substr(nCharSize, keyword.size());
	if (restChar.size() == 0)
	{
		firstNode->m_bIsFinish = true;
		return NULL;
	}

	return insert(firstNode, restChar);
}

WordNode* WordTree::insertBranch(WordNode* parent, string& keyword)
{
	if (keyword.size() == 0)
	{
		return NULL;
	}

	size_t nCharSize = GetUtf8CharSize((unsigned)keyword[0]);
	// 如果字节数不够了? 先给个打印看会不会出现这个情况
	if (keyword.size() < nCharSize)
	{
		cout << "读取配置文件时发现某个字符的字节数量不够!!!" << endl;
	}

	string firstChar = keyword.substr(0, nCharSize);
	WordNode* firstNode = parent->insertChild(firstChar);
	if (firstNode != NULL)
	{
		firstNode->m_parent = parent;
		string restChar = keyword.substr(nCharSize, keyword.size());
		if (!restChar.empty())
		{
			return insertBranch(firstNode, restChar);
		}
		else
		{
			// 一个检测单元结束
			firstNode->m_bIsFinish = true;

		}
	}
	return NULL;
}

WordNode* WordTree::find(WordNode* parent, string& keyword)
{
	if (keyword.size() == 0)
	{
		count = 0;
		return NULL;
	}

	size_t nCharSize = GetUtf8CharSize((unsigned)keyword[0]);
	if (keyword.size() < nCharSize)
	{
		cout << "检测时发现字符的字节数不够" << endl;
		count = 0;
		return NULL;
	}

	string firstChar = keyword.substr(0, nCharSize);
	WordNode* firstNode = parent->findChild(firstChar);
	// 如果检测失败了，看一下前面是否有可以完成检测的
	if (firstNode==NULL)
	{
		while (parent)
		{
			if (parent->m_bIsFinish)
			{
				return parent;
			}
			else
			{
				count -= parent->m_character.size();
				parent = parent->m_parent;
			}
		}
		
		count = 0;
		return NULL;
	}
	else
	{
		count += nCharSize;
	}

	string restChar = keyword.substr(nCharSize, keyword.size());
	if (firstNode->m_map.empty())
	{// 与配置文件的某一行完全匹配(完成检测) 可以退出函数
		return firstNode;
	}

	//如果检测到没有字符了 看一下前面是否有可以完成检测的。
	//因为默认是检测到失败为止
	//比如配置文件里有“你妹” 、“你妹啊”
	//如果你输入“你妹的” 检测到"你妹"之后不会停止检测，直到发现不匹配之后，再往前查看.
	if (restChar.size() == 0)
	{
		parent = firstNode;
		while (parent)
		{
			if (parent->m_bIsFinish)
			{
				return parent;
			}
			else
			{
				count -= parent->m_character.size();
				parent = parent->m_parent;
			}
		}

		count = 0;
		return NULL;
	}

	return find(firstNode, restChar);
}


bool CSensitiveWordFilter::load(const char* filepath)
{
	ifstream infile(filepath, ios::in);

	if (!infile)
	{
		cout << "open file error" << endl;
		return false;
	}
	else
	{
		cout << "open file succeed" << endl;
		string read;
		int nLoopCount = 0;
		while (getline(infile, read))
		{
			nLoopCount++;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			string s;
			s = read.substr(0, read.length());
			m_tree.insert(s);
#endif
		}
	}

	infile.close();
	return true;
}

bool CSensitiveWordFilter::censor(string &source, const string& replaceStr)
{
	bool bHasModified = false;
	int length = source.size();
	for (int i = 0; i < length;)
	{
		string substring = source.substr(i, length - i);
		if (m_tree.find(substring) != NULL)
		{
			source.replace(i, (m_tree.count), replaceStr);
			length = source.size();
			cout << "source = " << Utf82Ascii(source) << endl;
			bHasModified = true;
		}

		i += GetUtf8CharSize((unsigned)source[i]);
	}

	return bHasModified;
}
