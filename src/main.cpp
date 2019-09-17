#include "object.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;
int main(int argc, char** argv)
{
	if( argc != 4)
	{
		cout<<"Error Input"<<endl;
		return 0;
	}

	clock_t start = clock();

	Object myobj;
	myobj.loadObj(argv[1]);
	myobj.setRatio(atof(argv[3]));
	myobj.simplify();
	myobj.outputObj(argv[2]);
	
	clock_t end = clock();
	cout << "total:  "<< 1.0 * (end - start) / CLOCKS_PER_SEC << endl;
	return 0;
}