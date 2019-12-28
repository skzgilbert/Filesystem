#include "Table.h"

#include <iostream>
#include <string>

using namespace std;

int main(){
	Table table("disk1", 256, 128, "flatfile", "indexfile");
	table.Build_Table("data2.txt");

	string date;
	cout << "Enter data to search: ";
	cin >> date;
	cout << endl;
	cout << "Searching for date " << date << endl;
	table.Search(date);
	cout << endl;
	return 0;
}


