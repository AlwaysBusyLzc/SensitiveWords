/*---------------------------------------------------------------
�ļ�:		SensitiveWordFilter.h
�ļ�˵����	���дʼ���滻
���ߣ�		lzc
ʱ�䣺		10/14/2016
�汾��		1.0.0.0
---------------------------------------------------------------*/

#include <map>
#include <string>

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
	bool m_bIsFinish;			// һ����ⵥԪ�Ľ�����׼  true �����Ե���ֹͣ
	
public:
	WordNode(string character);
	WordNode(){
		m_character = "";
		m_bIsFinish = false;
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
	// ��Ⲣ�滻�ַ���  ����ԭ�ַ����Ƿ��޸�
	bool censor(string &source, const string& replaceStr = "**");
};
