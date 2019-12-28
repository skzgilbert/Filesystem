//Gilberto Pineda
//CSE 461
//Project
//November 25, 2019
//Sdisk.h

#ifndef SDISK_H
#define SDISK_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;


class Sdisk
{
public :
Sdisk(string diskName, int numberOfBlocks, int blockSize);
int getblock(int blocknumber, string& buffer);
int putblock(int blocknumber, string buffer);
int getnumberofblocks(); // accessor function
int getblocksize(); // accessor function
private :
string diskname;        // file name of software-disk
int numberofblocks;     // number of blocks on disk
int blocksize;          // block size in bytes
};

Sdisk::Sdisk(string diskName, int numberOfBlocks, int blockSize)
{
  this->diskname = diskName;
  this->numberofblocks = numberOfBlocks;
  this->blocksize = blockSize;

  fstream iofile;
  iofile.open(diskname.c_str(), ios::in);
  if(iofile.good()) {
    cout << "File Exist!" << endl;
  }
  else {
    cout << "File Does Not Exist, Creating File: " << diskname << endl;
    iofile.open(diskname.c_str(), ios::out);
    for (int i = 0; i < (blocksize*numberofblocks); i++) {
      iofile.put('#');
    }
  }
  iofile.close();
}

int Sdisk::getblock(int blocknumber, string& buffer)
{
  fstream infile;
  infile.open(diskname.c_str(), ios::in);
  infile.seekg(blocknumber*blocksize);
  if(infile.good()){
    for (int i = 0; i < blocksize; i++) {
      buffer += infile.get();
    }
    infile.close();
    return 1;
  }
  else {
    return 0;
  }
}

int Sdisk::putblock(int blocknumber, string buffer) 
{
  fstream ofile;
  ofile.open(diskname.c_str(), ios::in | ios::out);
  ofile.seekg(blocknumber*blocksize);
  if(ofile.good()) {
    for (int i = 0; i < buffer.length() && i < blocksize; i++) {
      ofile.put(buffer[i]);
    }
    ofile.close();
    return 1;
  }
  else {
    cout << "Failed to write " << diskname << endl;
    return 0;
  }
}

int Sdisk::getnumberofblocks() 
{
  return numberofblocks;
}

int Sdisk::getblocksize()
{
  return blocksize;
}

#endif
