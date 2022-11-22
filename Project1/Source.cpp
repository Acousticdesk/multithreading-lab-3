#include <windows.h>
#include <stdio.h>

const int numPhilosophers = 5;
const int maxPhilosophersAtTable = 4;

HANDLE hPhilosophersQueueSemaphore = CreateSemaphore(
	NULL,
	maxPhilosophersAtTable,
	maxPhilosophersAtTable,
	NULL // unnamed semaphore
); // allow only up to 4 philosophers at the table to prevent deadlock condition

HANDLE philosophers[numPhilosophers];
DWORD philosopherIdArray[numPhilosophers];

struct PhilosopherData {
	int index;
};

PhilosopherData philosophersData[numPhilosophers];

DWORD WINAPI dine(LPVOID param)
{
	PhilosopherData philosopherData = *(PhilosopherData *)param; // how to read it correctly?
	int philosopherIndex = philosopherData.index;

	printf("%lu\n\n", philosopherIndex);

	// WaitForSingleObject(hPhilosophersQueueSemaphore, INFINITE); // a philosopher won't be able to sit at the table if there are already 4 philosophers at the table
	// printf("Pholosopher %d started eating", philosopherIndex);

	return TRUE;
};

int WINAPI main()
{
	for (int i = 0; i < numPhilosophers; i++)
	{
		philosophersData[i].index = i;
		philosophers[i] = CreateThread(NULL, 0, dine, &philosophersData[i], NULL, &philosopherIdArray[i]);
	}

	WaitForMultipleObjects(numPhilosophers, philosophers, TRUE, INFINITE);

	return 0;
}