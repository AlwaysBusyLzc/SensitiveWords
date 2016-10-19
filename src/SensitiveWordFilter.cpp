#include "SensitiveWordFilter.h"
#include <iostream>
#include <fstream>

#define PACE 1

WordNode::WordNode(string character){
	if (character.size() == PACE)
	{
		m_character.assign(character);
		m_bIsFinish = false;
	}
	else
		cout << "error" << endl;
}

WordNode* WordNode::findChild(string& nextCharacter){
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

WordNode* WordNode::insertChild(string& nextCharacter){
	if (!findChild(nextCharacter))
	{
		m_map.insert(pair<string,WordNode>(nextCharacter, WordNode(nextCharacter)));
		return &(m_map.find(nextCharacter)->second);
	}
	return NULL;
}


WordNode* WordTree::insert(string &keyword){
	return insert(&m_emptyRoot, keyword);
}

WordNode* WordTree::insert(const char* keyword){
	string wordstr(keyword);
	return insert(wordstr);
}

WordNode* WordTree::find(string& keyword){
	return find(&m_emptyRoot,keyword);
}

WordNode* WordTree::insert(WordNode* parent, string& keyword){
	if (keyword.size() == 0)
	{
		parent->m_bIsFinish = true;
		return NULL;
	}
	string firstChar = keyword.substr(0, PACE);
	WordNode* firstNode = parent->findChild(firstChar);
	if (firstNode == NULL)
	{
		return insertBranch(parent,keyword);
	}
	string restChar = keyword.substr(PACE, keyword.size());
	return insert(firstNode, restChar);
}

WordNode* WordTree::insertBranch(WordNode* parent, string& keyword){
	string firstChar = keyword.substr(0, PACE);
	WordNode* firstNode = parent->insertChild(firstChar);
	if (firstNode != NULL)
	{
		string restChar = keyword.substr(PACE, keyword.size());
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

WordNode* WordTree::find(WordNode* parent, string& keyword){
	string firstChar = keyword.substr(0, PACE);
	WordNode* firstNode = parent->findChild(firstChar);
	if (firstNode==NULL)
	{
		// 如果父节点可以完成检测
		if (parent->m_bIsFinish)
		{
			count--;
			return parent;
		}
		
		count = 0;
		return NULL;
	}
	string restChar = keyword.substr(PACE, keyword.size());
	if (firstNode->m_map.empty())
	{
		return firstNode;
	}
	if (keyword.size()==PACE)
	{
		if (firstNode->m_bIsFinish)
		{
			return firstNode;
		}
		
		return NULL;
	}
	count++;
	return find(firstNode, restChar);
}


bool CSensitiveWordFilter::load(const char* filepath){
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

bool CSensitiveWordFilter::censor(string &source, const string& replaceStr){
	bool bHasModified = false;
	int lenght = source.size();
	for (int i = 0; i < lenght; i += 1)
	{
		string substring = source.substr(i, lenght - i);
		if (m_tree.find(substring) != NULL)
		{
			source.replace(i, (m_tree.count+1), replaceStr);
			lenght = source.size();
			cout << "source = " <<  source << endl;
			bHasModified = true;
		}
	}
	return bHasModified;
}
