// TestConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Echo/ReadWriteLock.h>

Echo::ReadWriteLock readWrite;

int GetLock()
{
	Echo::Lock<Echo::ReadWriteLock> lock(readWrite);
	return 1;
}

int main()
{
    return 0;   
}

