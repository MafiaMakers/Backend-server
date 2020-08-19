
#include "functions.h"

using namespace Mafia;
using namespace System;


MafiaList<void*> PointerManager::allocatedPointers = MafiaList<void*>();

bool PointerManager::hasPtr(void *pointer)
{
    return PointerManager::allocatedPointers.contains(pointer);
}

void PointerManager::addPtr(void *pointer)
{
	allocatedPointers.append(pointer);
}

void PointerManager::removePtr(void *pointer)
{
	if(allocatedPointers.contains(pointer)){
		allocatedPointers.removeOne(pointer);
	}
}
