//Gilberto Pineda
//CSE 461
//Project
//November 25, 2019
//Filesys.h

#ifndef FILESYS_H
#define FILESYS_H
#include "Sdisk.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;


class Filesys: public Sdisk
{
public :
Filesys(string diskname, int numberofblocks, int blocksize);
int fsclose();
int fssynch();
int newfile(string file);
int rmfile(string file);
int getfirstblock(string file);
int addblock(string file, string block);
int delblock(string file, int blocknumber);
int readblock(string file, int blocknumber, string& buffer);
int writeblock(string file, int blocknumber, string buffer);
int nextblock(string file, int blocknumber);
bool checkblock(string file, int blocknumber);
vector <string> ls();
private :
int rootsize;           // maximum number of entries in ROOT
int fatsize;            // number of blocks occupied by FAT
vector<string> filename;   // filenames in ROOT
vector<int> firstblock; // firstblocks in ROOT
vector<int> fat;             // FAT
};

Filesys::Filesys(string file, int numberofblocks, int blocksize):Sdisk(file, numberofblocks, blocksize)
{
	string buffer;
	rootsize = getblocksize()/13;
	fatsize = getnumberofblocks()*4/getblocksize() + 1;
	
	getblock(1, buffer);
	if(buffer[1] == '#')
	{
		cout << "No file system!" << endl;
		
		for (int i = 0; i < rootsize; i++)
		{
			filename.push_back("XXXXXXXX");
			firstblock.push_back(0);
		}
		fat.push_back(fatsize+2);
		
		for(int i = 1; i <= fatsize + 1; i++)
		{
			fat.push_back(-1);
		}
	
		for(int i = 2 + fatsize; i < getnumberofblocks(); i++)
		{
			fat.push_back(i+1);
		}
		fat[fat.size() - 1] = 0;
		cout << "Created file system!" << endl;
	}

	else
	{
		cout << "File system exist!" << endl;
		string buffer;
		istringstream readroot;
		readroot >> buffer;

		for(int i = 0; i < rootsize; i++)
		{
			string x;
			int n;

			readroot >> x >> n;
			filename.push_back(x);
			firstblock.push_back(n);
		}

		string fatbuffer;
		for(int i = 0; i < fatsize; i++)
		{
			string x;
			
			getblock(i + 2, x);
			fatbuffer += x;
		}
		
		istringstream fatstream;
		fatstream.str(fatbuffer);
	
		for(int i = 0; i < getnumberofblocks(); i++)
		{
			int temp;
			
			fatstream >> temp;
			fat.push_back(temp);
		}
	}

	fssynch();
}


vector <string> block(string buffer, int n)
{
	vector<string> blocks;
	int numberofblocks = 0;

	if(buffer.length() % n == 0)
	{
		numberofblocks = buffer.length()/n;
	}
	else
	{
		numberofblocks = buffer.length()/n + 1;
	}

	string tblock;
	for(int i = 0; i < numberofblocks; i++)
	{
		tblock = buffer.substr(n*i, n);
		blocks.push_back(tblock);
	}

	int lblock = blocks.size() - 1;

	for (int i = blocks[lblock].length(); i < n; i++)
	{
		blocks[lblock] += "#";
	}
	
	return blocks;
}

int Filesys::fsclose()
{
	fssynch();
	return 0;
}

int Filesys::fssynch()
{
	ostringstream rootstream;

	for(int i = 0; i < rootsize; ++i)
	{
		rootstream << filename[i] << " " << firstblock[i] << " ";
	}
	
	string root = rootstream.str();
	for(long k = root.size(); k < getblocksize(); k++)
	{
		root += "#";
	}
	putblock(1, root);

	ostringstream fatstream;

	for(int i = 0; i < getnumberofblocks(); i++)
	{
		fatstream << fat[i] << " ";
	}
	
	string fstring = fatstream.str();
	
	vector <string> blocks = block(fstring, getblocksize());

	for(int i = 0; i < blocks.size(); i++)
	{
		putblock(i + 2, blocks[i]);
	}

	return 1;
}

int Filesys::newfile(string file)
{
	for(int i = 0; i < filename.size(); i++)
	{
		if(filename[i] == file)
		{
			cout << "File " << file << " already exists!" << endl;
			return 0;
		}
	}

	for(int i = 0; i < filename.size(); i++)
	{
		if(filename[i] == "XXXXXXXX")
		{
			cout << "Creating File " << file  << endl;
		 	filename[i] = file;
			firstblock[i] = 0;
			break;
		}
	}
	fssynch();
	return 1;
}

int Filesys::rmfile(string file)
{
	for(int i = 0; i < filename.size(); i++)
	{
		if(firstblock[i] != 0)
		{
			cout << "File Not Empty!" << endl;
		}
		if(filename[i] == file && firstblock[i] == 0)
		{
			filename[i] = "XXXXXXXX";
			firstblock[i] = 0;
			fssynch();
			return 1;
		}
	}
	cout << "File Does Not Exist!" << endl;
	return 0;
}

int Filesys::getfirstblock(string file)
{
	int fblock = -1;
	for(int i = 0; i < filename.size(); i++)
	{
		if(filename[i] == file)
		{
			fblock = firstblock[i];
			break;
		}
	}
	fssynch();
	return fblock;
}

bool Filesys::checkblock(string file, int blocknumber)
{
	int iblock = getfirstblock(file);
	while(iblock != 0)
	{
		if (iblock == blocknumber)
		{
			return true;
		}
		iblock = fat[iblock];
	}
	return false;
}

int Filesys::addblock(string file, string buffer)
{
	int firstblock = getfirstblock(file), newBlock = fat[0];
	if(newBlock == 0)
	{
		cout << "Disk full!" << endl;
		return -1;
	}
	
	if(firstblock == 0)
	{
		for(int i = 0; i < filename.size(); i++)
		{
			if(filename[i] == file)
			{
				firstblock = newBlock;
				fat[0] = fat[newBlock];
				fat[newBlock] = 0;
			}
		}
	}
	
	else
	{
		int nextBlock = firstblock;
		while(fat[newBlock] != 0)
		{
			nextBlock = fat[nextBlock];
		}
		fat[nextBlock] = nextBlock;
		fat[0] = fat[newBlock];
		fat[newBlock] = 0;
	}

	putblock(newBlock, buffer);
	fssynch();
	return newBlock;
}

int Filesys::delblock(string file, int blocknumber)
{
	if(!checkblock(file, blocknumber))
	{
		return 0;
	}

	if(blocknumber == getfirstblock(file))
	{
		for (int i = 0; i < filename.size(); i++)
		{
			if(filename[i] == file)
			{
				firstblock[i] = fat[blocknumber];
			}
		}
	}
	else
	{
		int blockID = getfirstblock(file);
		while(fat[blockID] != blocknumber)
		{
			blockID = fat[blockID];
		}
		fat[blockID] = fat[blocknumber];
	}
	
	fat[blocknumber] = fat[0];
	fat[0] = blocknumber;
	fssynch();
}

int Filesys::readblock(string file, int blocknumber, string& buffer)
{
	if(checkblock(file, blocknumber))
	{
		getblock(blocknumber, buffer);
		return 1;
	}
	else
	{
		return 0;
	}
}	

int Filesys::writeblock(string file, int blocknumber, string buffer)
{
	putblock(blocknumber, buffer);
	return 1;
}

int Filesys::nextblock(string file, int blocknumber)
{
	int blockID = getfirstblock(file);
	while(blockID != blocknumber)
	{
		blockID = fat[blockID];
	}
	return fat[blockID];
}

vector <string> Filesys::ls()
{
	vector <string> list;
	for(int i = 0; i < filename.size(); i++)
	{
		if (filename[i] != "XXXXXXXX")
		{
			list.push_back(filename[i]);
		}
	}
	return list;
}


#endif
