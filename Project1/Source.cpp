#include <windows.h>
#include <stdio.h>

/// <summary>
/// Dining Philosophers Problem using WinAPI
/// by Andrii Kicha 23.11.2022
/// 
/// To solve the deadlock problem, a number of the philosophers at the table is restricted to N - 1
/// 
/// 
/// </summary>

const int numPhilosophers = 5;
const int maxPhilosophersAtTable = 4;

HANDLE hPhilosophersQueueSemaphore = CreateSemaphore(
	NULL, // security flags
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

HANDLE forks[numPhilosophers];

// left fork index
int left(int i)
{
	return i;
}

// right fork index
int right(int i)
{
	return (i + numPhilosophers - 1) % numPhilosophers;
}

DWORD WINAPI dine(LPVOID param)
{
	PhilosopherData philosopherData = *(PhilosopherData *)param; // how to read it correctly?
	int philosopherIndex = philosopherData.index;

	printf("A philosopher %lu approaches the table\n", philosopherIndex);

	WaitForSingleObject(hPhilosophersQueueSemaphore, INFINITE); // a philosopher won't be able to sit at the table if there are already 4 philosophers at the table
	// if we are here, it means that the number of the philosophers at the table are less then or equals to N-1

	printf("A philosopher %lu sits at the table\n", philosopherIndex);

	int leftForkIndex = left(philosopherIndex);
	// Take a left fork
	WaitForSingleObject(forks[leftForkIndex], INFINITE);
	printf("A philosopher %lu takes a left fork %d\n", philosopherIndex, leftForkIndex);

	int rightForkIndex = right(philosopherIndex);
	// Take a right fork
	WaitForSingleObject(forks[rightForkIndex], INFINITE);
	printf("A philosopher %lu takes a right fork %d\n", philosopherIndex, rightForkIndex);

	printf("A philosopher %lu is eating\n", philosopherIndex);
	Sleep(1000);

	printf("A philosopher %lu finished eating\n", philosopherIndex);
	ReleaseSemaphore(forks[leftForkIndex], 1, NULL);
	printf("A philosopher %lu put left fork %d\n", philosopherIndex, leftForkIndex);
	ReleaseSemaphore(forks[rightForkIndex], 1, NULL);
	printf("A philosopher %lu put right fork %d\n", philosopherIndex, rightForkIndex);

	ReleaseSemaphore(hPhilosophersQueueSemaphore, 1, NULL);
	printf("A philosopher %lu left the table\n", philosopherIndex);

	return TRUE;
};

int WINAPI main()
{
	// initialize philosophers
	for (int i = 0; i < numPhilosophers; i++)
	{
		philosophersData[i].index = i;
		philosophers[i] = CreateThread(NULL, 0, dine, &philosophersData[i], NULL, &philosopherIdArray[i]);
	}

	// initalize forks
	for (int i = 0; i < numPhilosophers; i++)
	{
		forks[i] = CreateSemaphore(NULL, 1, 1, NULL);
	}

	WaitForMultipleObjects(numPhilosophers, philosophers, TRUE, INFINITE);

	return 0;
}