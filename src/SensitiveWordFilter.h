/*---------------------------------------------------------------
文件:		SensitiveWordFilter.h
文件说明：	敏感词检测替换
作者：		lzc
时间：		10/14/2016
版本：		1.0.0.0
---------------------------------------------------------------*/

#include <map>
#include <string>
#include <algorithm>
#include "windows.h"

using namespace std;

class WordTree;
class WordNode
{
private:
	friend class WordTree;
	typedef map<string, WordNode> _TreeMap;
	typedef map<string, WordNode>::iterator _TreeMapIterator;

	string m_character;
	_TreeMap m_map;
	WordNode* m_parent;
	bool m_bIsFinish;			// 一个检测单元的结束标准  true 检测可以到此停止
	
public:
	WordNode(string character);
	WordNode(){
		m_character = "";
		m_bIsFinish = false;
		m_parent = NULL;
	};
	string getCharacter() const{ return m_character; };
	WordNode* findChild(string& nextCharacter);
	WordNode* insertChild(string& nextCharacter);
};

class WordTree
{
public:
	int count;
	WordNode* insert(string &keyword);
	WordNode* insert(const char* keyword);
	WordNode* find(string& keyword);
	WordTree(){ count = 0; };

private:
	WordNode m_emptyRoot;
	int m_pace;
	WordNode* insert(WordNode* parent, string& keyword);
	WordNode* insertBranch(WordNode* parent, string& keyword);
	WordNode* find(WordNode* parent, string& keyword);
};

class CSensitiveWordFilter
{
public:
	CSensitiveWordFilter(){}
	~CSensitiveWordFilter(){}
	
private:
	WordTree m_tree;

public:
	bool load(const char* filepath);
	// 检测并替换字符串  返回原字符串是否被修改
	bool censor(string &source, const string& replaceStr = "**");
};
