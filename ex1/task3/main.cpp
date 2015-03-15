#include <iostream>
#include "osm.h"

using namespace std;

int main()
{
    
	for(int i = 0; i < 99; i++)
	{
		timeMeasurmentStructure measurements = measureTimes(1000000);
		cout<< "machine name: " << measurements.machineName << endl;
		cout<< "instruction time (nanoSeconds): " << measurements.instructionTimeNanoSecond << endl;
		cout<< "empty function call time(nanoSeconds): " << measurements.functionTimeNanoSecond << endl;
		cout<< "trap time (nanoSeconds): " << measurements.trapTimeNanoSecond << endl;
		cout<< "function - instruction ratio: " << measurements.functionInstructionRatio << endl;
		cout << "trap - instruction ratio: " << measurements.trapInstructionRatio << endl;
	}
    return 0;

}
