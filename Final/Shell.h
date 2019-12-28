//Gilberto Pineda
//CSE 461
//Project
//November 25, 2019
//Shell.h
#ifndef SHELL_H
#define SHELL_H

#include "Filesys.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

class Shell: public Filesys
{
public :
Shell(string filename, int numberofblocks, int blocksize);
int dir();// lists all files
int add(string file);// add a new file using input from the keyboard
int del(string file);// deletes the file
int type(string file);//lists the contents of file
int copy(string file1, string file2);//copies file1 to file2
};


Shell::Shell(string filename, int numberofblocks, int blocksize):Filesys(filename, numberofblocks, blocksize)
{}

int Shell::dir()
{
	vector<string> fileList = ls();
	for(int i = 0; i < fileList.size(); i++)
	{
		cout << fileList[i] << endl;
	}
	return 1;
}

int Shell::del(string file)
{
	int block = getfirstblock(file);
	while(block > 0)
	{
		delblock(file, block);
		block = getfirstblock(file);
	}
	rmfile(file);
	return 1;
}

int Shell::copy(string file1, string file2)
{
	int code = getfirstblock(file1);
	if(code == -1)
	{
		cout << "File " << file1 << " does not exist!" << endl;
		return 0;
	}
	int code2 = getfirstblock(file2);
	if(code2 != -1)
	{
		cout << "File " << file2 << " exist!" << endl;
		return 0;
	}
	
	int code3 = newfile(file2);
	if(code3 == 0)
	{
		cout << "No space in root!" << endl;
		return 0;
	}
	
	string buffer;
	while(code != 0)
	{
		string b;
		code3 = readblock(file1, code, b);
		buffer += b;
		addblock(file2, buffer);
		code = nextblock(file1, code);
	}
}

int Shell::type(string file)
{
	int block = getfirstblock(file);
	string buffer;
	
	while(block > 0)
	{
		string b;
		int code = readblock(file, block, b);
		buffer += b;
		code = nextblock(file, block);
	}
	cout << buffer << endl;
	cout << buffer.length() << endl;
	return 1;
}

int Shell::add(string file)
{
	newfile(file);
	string zfile;

	for(int i  = 0; i < 128; i++)
	{
		zfile += "r";
	}
	vector <string> blocks = block(zfile, 128);
	
	for(int i = 0; i < blocks.size(); i++)
	{
		addblock(file, blocks[i]);
	}
	return 1;
}

#endif
