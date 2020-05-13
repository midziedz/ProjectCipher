#include <gtk/gtk.h>
#include <fstream>
#include <string>
using namespace std;
#include "../include/iomanager.h"

IOManager::IOManager()
{
	this->fileName = "";
	this->ioAction = IOManager::IOAction::Inactive;
}

bool IOManager::setFile(string fileName, IOManager::IOAction ioAction)
{
	this->fileName = fileName;
	this->ioAction = ioAction;
	if(this->ioAction != IOManager::IOAction::Inactive && this->fileName != "") this->fileName = fileName.substr(7, this->fileName.length() - 1);
	return true;
}

bool IOManager::loadToString(string& Input)
{
	switch(this->ioAction)
	{
		case 0:
		{
			string input = "";
    		string line = "";
    		ifstream inputFile(this->fileName.c_str());
    		while(!inputFile.eof())
    		{
    			getline(inputFile, line);
    			input = input + line + "\n";
    		}
    		inputFile.close();
    		if(input[input.length() - 1] == '\n') input = input.substr(0, input.length() - 1);
    		Input = input;
    		return true;
    	}
    	default:
    	{
    		return false;
    	}
    }
}

bool IOManager::saveFromString(string& output)
{
	switch(this->ioAction)
	{
		case 1:
		{
    		ofstream outputFile(this->fileName.c_str());
			outputFile << output;
    		outputFile.close();
			return true;
		}
		default:
		{
			return false;
		}
	}
}
