#include <gtk/gtk.h>
#include <fstream>
#include <string>
using namespace std;
#include "../include/cyphermanager.h"
CypherManager::CypherManager()
{
	this->fileName = "";
	this->fileType = CypherManager::FileType::JustFile;
}
CypherManager::CypherManager(string fileName, CypherManager::FileType fileType)
{
	this->fileName = fileName;
	this->fileType = fileType;
}

bool CypherManager::setFile(string fileName, CypherManager::FileType fileType)
{
	this->fileName = fileName;
	this->fileType = fileType;
	if(this->fileType == CypherManager::FileType::JustVigenereFile) this->fileName = fileName.substr(7, this->fileName.length() - 1);
	return true;
}

bool CypherManager::flushToCypher(VigenereCypher& vigenereCypher) const
{
	switch(this->fileType)
	{
		case 0:
		{
			ifstream inputFile(this->fileName.c_str());
			string signSet = "";
			getline(inputFile, signSet);
			inputFile.close();
			signSet = signSet + "\n";
			vigenereCypher.setTableKeyAndSize(signSet);
			return true;
		}
		case 1:
		{
			ifstream inputFile(this->fileName.c_str());
			string firstVerse = "";
			string VigenereKeyFileName = "";
    		getline(inputFile, firstVerse);
    		getline(inputFile, VigenereKeyFileName);
    		inputFile.close();
    		firstVerse = firstVerse + "\n";
    		vigenereCypher.setTableKeyAndSize(firstVerse);
    		vigenereCypher.setVigenereKeyFileName(VigenereKeyFileName);
			return true;
		}
		case 2:
		{
			vigenereCypher.setTableKeyAndSize(this->fileName);
			return true;
		}
		case 3:
		{
			vigenereCypher.setVigenereKeyFileName(this->fileName);
			return true;
		}
		default:
		{
			return false;
		}
	}
}

bool CypherManager::saveFromCypher(VigenereCypher& vigenereCypher)
{
	switch(this->fileType)
	{
		case 0:
		{
			ofstream outputFile(this->fileName.c_str());
			outputFile << vigenereCypher.getTableKey();
			outputFile.close();
			return true;
		}
		case 1:
		{
			ofstream outputFile(this->fileName.c_str());
    		outputFile << vigenereCypher.getTableKey();
    		outputFile << vigenereCypher.getVigenereKeyFileName();
    		outputFile.close();
			return true;
		}
		case 2:
		case 3:
		{
			return false;
		}
		default:
		{
			return false;
		}
	}
}
