#ifndef _ALLOCTOR_H_
#define _ALLOCTOR_H_

#include <memory_mgr.h>

void* operator new(std::size_t nSize);
void operator delete(void* p);
void* operator new[](std::size_t nSize);
void operator delete[](void* p);

#endif