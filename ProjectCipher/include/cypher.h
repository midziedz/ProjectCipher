#ifndef CYPHER_H
#define CYPHER_H

#include <fstream>
#include <string>
//#include <locale>
//#include <codecvt>
using namespace std;

class VigenereCypher
{
    private:
    unsigned int tableKeySize;
    string *tableKey;
    string VigenereKeyFileName;
    string output;
    public:
    // Constructors:
    VigenereCypher();
    // Setters:
    bool setTableKeyAndSize(string firstVerse);
    bool setVigenereKeyFileName(string VigenereKeyFileName);
    // Polish (and more) diacritics conversion from/to typical, "narrow" string:
    /*wstring widenUp(string narrowString)
	{
		wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
		wstring wide = converter.from_bytes(narrowString);
		return wide;
	}
	string narrowDown(wstring wideString)
	{
		wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
		string narrow = converter.to_bytes(wideString);
		return narrow;
	}*/
    string getTableKey()
    {
    	return this->tableKey[0];
    }
    string getVigenereKeyFileName()
    {
    	return this->VigenereKeyFileName;
    }
    // Actual cypher/decypher functions:
    string cypher(string input);
    string decypher(string input);
};
#endif
