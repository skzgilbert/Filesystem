#include "Filesys.h"
#include "Sdisk.h"
#include "Shell.h"
#include "Table.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
 //
 //This main program inputs commands to the shell.
 //It inputs commands as : command op1 op2
 //You should modify it to work for your implementation.
 //
Shell shell("disk1", 256, 128);
Table table("disk2", 256, 128, "flatfile", "indexfile");
table.Build_Table("data2.txt"); 
string s;
 string command="go";
 string op1,op2;

 while (command != "quit")
     {
       command.clear();
       op1.clear();
       op2.clear();
       cout << "$";
       getline(cin,s);
       int firstblank=s.find(' ');
       if (firstblank < s.length()) s[firstblank]='#';
       int secondblank=s.find(' ');
       command=s.substr(0,firstblank);
       if (firstblank < s.length())
         op1=s.substr(firstblank+1,secondblank-firstblank-1);
       if (secondblank < s.length())
         op2=s.substr(secondblank+1);
       if (command=="dir")
          {
            shell.dir();
           }
       if (command=="add")
          {
             shell.add(op1);
           }
       if (command=="del")
          {
            shell.del(op1);
           }
       if (command=="type")
          {
            shell.type(op1);
          }
       if (command=="copy")
          {
            shell.copy(op1, op2);
          }
	       if (command=="search")
          {
           table.Search(op1);
          }
       
      }

 return 0;
}
