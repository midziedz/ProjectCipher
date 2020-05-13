#ifndef CYPHER_MANAGER_H
#define CYPHER_MANAGER_H

#include <fstream>
#include <string>
using namespace std;
#include "../include/cypher.h"

class CypherManager
{
	private:
	string fileName;
	public:
	enum FileType
	{
		SignSet = 0,
		KeySet = 1,
		JustSignSet = 2,
		JustVigenereFile = 3,
		JustFile = 4
	};
	private:
	FileType fileType;
	public:
	CypherManager();
	CypherManager(string fileName, FileType fileType);
	bool setFile(string fileName, FileType fileType);
	bool flushToCypher(VigenereCypher& vigenereCypher) const;
	bool saveFromCypher(VigenereCypher& vigenereCypher);
	~CypherManager()
	{

	}
};

#endif
