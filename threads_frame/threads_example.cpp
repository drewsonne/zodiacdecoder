#include <windows.h> <WINDOWS.H>
#include <strsafe.h> <STRSAFE.H>
#include <iostream>
#define NUM_THREADS 4
#define THERE_IS_TIME_LEFT TRUE


DWORD WINAPI FunctionToPassToThread ( LPVOID lpParam ) 
{
	/* This is the function that gets its own thread.
	
	CallToYourAlgorithmGoesHere();

	*/
	return (0);
} 
 
void main()
{
	int DataToPassToFunction = 1;						// can use a struct to pass more information
	HANDLE ThreadHandles[NUM_THREADS];

	for (int x = 0; x < NUM_THREADS; x++) {				// create all initial threads
		ThreadHandles[x] = CreateThread( NULL, 0, FunctionToPassToThread, &DataToPassToFunction, 0, NULL);  
	}

	while (THERE_IS_TIME_LEFT) {
		// Wait until all threads have terminated.
		WaitForMultipleObjects( 3, ThreadHandles, TRUE, INFINITE);

		for (int x = 0; x < NUM_THREADS; x++) { // Close and then recreate threads
			CloseHandle(ThreadHandles[x]);
			ThreadHandles[x] = CreateThread( NULL, 0, FunctionToPassToThread, &DataToPassToFunction, 0, NULL);  
		}
	}
}