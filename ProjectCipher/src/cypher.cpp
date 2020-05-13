#include <gtk/gtk.h>
#include <iostream>
#include <fstream>
#include <string>
//#include <locale>
//#include <codecvt>
#include <ctype.h>
using namespace std;
#include "../include/cypher.h"

VigenereCypher::VigenereCypher()
{
    this->tableKeySize = 0;
    this->tableKey = NULL;
    this->VigenereKeyFileName = "";
    this->output = "";
}

bool VigenereCypher::setTableKeyAndSize(string firstVerse)
{
	//wstring wFirstVerse = widenUp(firstVerse);
	int reduction = 0;
	for(unsigned int i = 0; i < firstVerse.length(); i++)
	{
		if(!isprint(firstVerse[i]) && firstVerse[i] != '	')
		{
			reduction += 1;
			//cout << firstVerse[i] << endl;
		}
	}
	reduction = reduction/2;
    this->tableKeySize = firstVerse.length() - reduction;
    if(firstVerse == "")
    {
    	delete[] this->tableKey;
    	this->tableKey = NULL;
    	return false;
    }
    if(this->tableKey != NULL) delete[] this->tableKey;
    this->tableKey = new string[tableKeySize];
    this->tableKey[0] = firstVerse;
    //cout << this->tableKey[0];
    for(unsigned int i = 1; i < this->tableKeySize; i++)
    {
    	if(!isprint(this->tableKey[i - 1][0]) && this->tableKey[i - 1][0] != '	')
    	{
    		//cout << (this->tableKey[i - 1].substr(2, firstVerse.length()) + this->tableKey[i - 1][0] + this->tableKey[i - 1][1]) << endl;
        	this->tableKey[i] = (this->tableKey[i - 1].substr(2, firstVerse.length()) + this->tableKey[i - 1][0] + this->tableKey[i - 1][1]);
    		continue;
    	}
    	//cout << (this->tableKey[i - 1].substr(1, firstVerse.length()) + this->tableKey[i - 1][0]) << endl;
        this->tableKey[i] = (this->tableKey[i - 1].substr(1, firstVerse.length()) + this->tableKey[i - 1][0]);
    }
    //cout << this->tableKey[0] << endl;
    return true;
}
bool VigenereCypher::setVigenereKeyFileName(string VigenereKeyFileName)
{
    this->VigenereKeyFileName = VigenereKeyFileName;
    //cout << this->VigenereKeyFileName << endl;
    return true;
}

string cutToNarrow(string input, string from, string to)
{
	//cout << input << endl;
	string output = "";
	for(unsigned int i = 0; i < input.length(); i++)
	{
		bool isNarrow = true;
		for(unsigned int j = 0; j < from.length(); j += 2)
		{
			if(input[i] == from[j] && input[i + 1] == from[j + 1])
			{
				//cout << input[i] << input[i + 1] << " " << from[j] << from[j + 1] << endl;
				output += to[(j / 2)];
				isNarrow = false;
				i++;
			}
		}
		/*int x = from.find(input[i]);
		cout << "x = " << x << endl;
		if(x >= 0)
		{
			output += to[(x / 2)];
			i++;
		}*/
		if(isNarrow) output += input[i];
	}
	//cout << output << endl;
	return output;
}

string VigenereCypher::cypher(string input)
{
    GtkWidget *dialog;
    //wstring wInput = widenUp(input);
    input = cutToNarrow(input, "ĄĆĘŁŃÓŚŻŹąćęłńóśżź", "ACELNOSZZacelnoszz");
    //cout << input << endl;
    if(this->tableKey[0] == "" || this->VigenereKeyFileName == "")
    {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Cypher key(s) invalid!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return "Error: Cypher key(s) invalid!";
    }
    ifstream VigenereKeyFile(this->VigenereKeyFileName.c_str());
    VigenereKeyFile.seekg(0, ios_base::end);
    unsigned int VigenereKeyFileSize = VigenereKeyFile.tellg();
    //VigenereKeyFileSize = VigenereKeyFileSize - 1;
    VigenereKeyFile.seekg(0, ios_base::beg);
    char singleKeySign;
    this->output = "";
    unsigned int inputLength = input.length();
    //cout << tableKey[0] << " " << VigenereKeyFileName << endl;
    for(unsigned int i = 0; i < inputLength; i++)
    {
        VigenereKeyFile.seekg(i%VigenereKeyFileSize, ios_base::beg);
        VigenereKeyFile.get(singleKeySign);
        string::size_type x = this->tableKey[0].find(input[i]);

        string::size_type y = this->tableKey[0].find(singleKeySign);
        //cout << "OK, up to now it works!" << endl;
        if(x == string::npos)
        {
            //cout << "Warning!" <<endl;
            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Input character not found in key set!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return "Error: Key set is invalid!";
        }
        //cout << x << " " << y << endl;
        if(y == string::npos)
        {
            //cout << "Warning!" <<endl;
            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Vigenere key character not found in key set!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return "Error: Key set or key file is invalid!";
        }
		if(!isprint(this->tableKey[x][y]) && this->tableKey[x][y] != '	' && this->tableKey[x][y] != '\n' && !isprint(this->tableKey[x - 1][y]) && this->tableKey[x - 1][y] != '	' && this->tableKey[x - 1][y] != '\n')
		{
			this->output += this->tableKey[x - 1][y];
		}
        this->output += this->tableKey[x][y];
        if(!isprint(this->tableKey[x][y]) && this->tableKey[x][y] != '	' && !isprint(this->tableKey[x + 1][y]) && this->tableKey[x + 1][y] != '	')
		{
			this->output += this->tableKey[x + 1][y];
		}
		if(!isprint(input[i]) && input[i] != '	' && input[i] != '\n') i++;
        //cout << input[i] << ", " << singleKeySign << " -> " << this->tableKey[x][y] << endl;
    }
    VigenereKeyFile.close();
    return this->output;
}

string VigenereCypher::decypher(string input)
{
    GtkWidget *dialog;
    input = cutToNarrow(input, "ĄĆĘŁŃÓŚŻŹąćęłńóśżź", "ACELNOSZZacelnoszz");
    //wstring wInput = widenUp(input);
    if(this->tableKey[0] == "" || this->VigenereKeyFileName == "")
    {
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Cypher key(s) invalid!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return "Error: Cypher key(s) invalid!";
    }
    ifstream VigenereKeyFile(this->VigenereKeyFileName.c_str());
    VigenereKeyFile.seekg(0, ios_base::end);
    unsigned int VigenereKeyFileSize = VigenereKeyFile.tellg();
    //VigenereKeyFileSize = VigenereKeyFileSize - 1;
    VigenereKeyFile.seekg(0, ios_base::beg);
    char singleKeySign;
    this->output = "";
    unsigned int inputLength = input.length();
    for(unsigned int i = 0; i < inputLength; i++)
    {
        VigenereKeyFile.seekg(i%VigenereKeyFileSize, ios_base::beg);
        VigenereKeyFile.get(singleKeySign);
        string::size_type y = this->tableKey[0].find(singleKeySign);
        string::size_type x = this->tableKey[y].find(input[i]);
        //cout << "OK, up to now it works!" << endl;
        if(x == string::npos)
        {
            //cout << "Warning!" <<endl;
            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Input character not found in key set!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return "Error: Key set is invalid!";
        }
        //cout << x << " " << y << endl;
        if(y == string::npos)
        {
            //cout << "Warning!" <<endl;
            dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Vigenere key character not found in key set!");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return "Error: Key set or key file is invalid!";
        }
        this->output += this->tableKey[0][x];
        //cout << this->tableKey[0] << endl;
        //cout << input[i] << ", " << singleKeySign << " -> " << this->tableKey[0][x] << endl;
    }
    VigenereKeyFile.close();
    return this->output;
}
