//Gilberto Pineda
//CSE 461
//Project
//Table.h
//



#ifndef TABLE_H
#define TABLE_H


#include "Filesys.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;


class Table: public Filesys
{
public :
Table(string diskname,int numberofblocks,int blocksize, string flatfile, string indexfile);
int Build_Table(string input_file);
int Search(string value);
private :
string flatfile;
string indexfile;
int numberofrecords;
//Filesys filesystem;
int IndexSearch(string value);
};

Table::Table(string diskname, int numberofblocks, int blocksize, string flatfile, string indexfile):Filesys(diskname, numberofblocks, blocksize)
{
	this->indexfile = indexfile;
	this->flatfile = flatfile;
	newfile(flatfile);
	newfile(indexfile);
}


int Table::Build_Table(string input_file){
	ifstream infile;
	infile.open(input_file.c_str());
	vector<string>iKey;
	vector<int>iBlock;
	string record;
	getline(infile, record);
	while(infile.good()){
		string pKey = record.substr(0, 5);
		vector<string>blocks = block(record, getblocksize());
		int blockID = addblock(flatfile, blocks[0]);
		iKey.push_back(pKey);
		iBlock.push_back(blockID);
		getline(infile, record);
		if(infile.bad() && iKey.size() > 0){
			ostringstream iBuffer;
			iBuffer << iKey.size() << " ";
			for(int i  = 0; i < iKey.size(); i++){
				iBuffer << iKey[i] << " " << iBlock[i] << " ";
			}
			string buffer = iBuffer.str();
			vector<string> block2 = block(buffer, getblocksize());
			//int code = addblock(indexfile, block2[0]);
			iKey.clear();
			iBlock.clear();
		}
	}
}
/*
int Table::IndexSearch(string value){
	int block = getfirstblock(indexfile);
	while(block != 0){
		string buffer;
		readblock(indexfile, block, buffer);
		istringstream istream;
		istream.str(buffer);
		for(int i = 0; i < 4; i++){
			string file;
			int id;
			istream >> file >> id;
			if(value == file){
				return id;
			}
		}
		block = nextblock(indexfile, block);
	}
	return 0;
}

int Table::Search(string value){
	int block = IndexSearch(value);
	if(block == 0){
		cout << "No such record." << endl;
	}
	else{
		string buffer;
		readblock(indexfile, block, buffer);
		cout << buffer << endl;
	}
}
*/
int Table::IndexSearch(string value){
	istringstream instream;
	int blockid = getfirstblock(indexfile);
	
	if(blockid == -1){
		cout << "Index not found!" << endl;
		return 0;
	}
	while(blockid != 0){
		string buffer, k;
		int b;
		readblock(indexfile, blockid, buffer);

		instream.str(buffer);

		for(int i = 0; i <= 4; ++i){
			instream >> k >> b;
			if( k == value){
				return b;
			}
		}
		blockid = nextblock(indexfile, blockid);
	}
	return 0;
}

int Table::Search(string value){
	int blockid = IndexSearch(value);
	
	if(blockid == 0){
		cout << "No record!" << endl;
	}
	else{
		string buffer;
		readblock(flatfile, blockid, buffer);
		
		while(buffer[buffer.size() - 1] == '#'){
			buffer.pop_back();
		}
		cout << buffer << endl;
		return 1;
	}
	
	return 0;
}

#endif
