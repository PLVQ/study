#include "alloctor.h"

void* operator new(std::size_t nSize)
{
    return MemoryMgr::Instance().alloctMem(nSize);
}
void operator delete(void* p)
{
    MemoryMgr::Instance().freeMem(p);
}
void* operator new[](std::size_t nSize)
{
    return MemoryMgr::Instance().alloctMem(nSize);
}
void operator delete[](void* p)
{
    MemoryMgr::Instance().freeMem(p);
}