#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <fstream>
#include <string>
using namespace std;

class IOManager
{
	private:
	string fileName;
	public:
	enum IOAction
	{
		Load = 0,
		Save = 1,
		Inactive = 2
	};
	private:
	IOAction ioAction;
	public:
	IOManager();
	bool setFile(string fileName, IOAction ioAction);
	bool loadToString(string& Input);
	bool saveFromString(string& Output);
	~IOManager()
	{
		
	}
};

#endif
