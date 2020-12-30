#ifndef _MEMORY_MGR_H_
#define _MEMORY_MGR_H_

#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <new>
#include <mutex>

class MemoryPool;
class MemoryBlock
{
public:
    int nId;
    int nRet;
    bool bPool;
    MemoryBlock* pNext;
    MemoryPool* pPool;

public:
    MemoryBlock();
    ~MemoryBlock();
};

class MemoryPool
{
protected:
    size_t m_nBlockCount;
    size_t m_nBlockSize;
    char* m_pBuf;
    MemoryBlock* m_pHeader;

public:
    MemoryPool();
    ~MemoryPool();
    void initMemPool();
    void* alloctMem(std::size_t nSize);
    void freeMem(void* p);
};

template<size_t nBlockSize, size_t nBlockCount>
class InitMemoryPool : public MemoryPool
{
private:
public:
    InitMemoryPool()
    {
        const size_t n = sizeof(void *);
        m_nBlockSize = ((nBlockSize / n) * n) + ((nBlockSize % n) ? n : 0);
        m_nBlockCount = nBlockCount;
    }
    ~InitMemoryPool()
    {

    }
};


class MemoryMgr
{
private:
    InitMemoryPool<64, 10> m_memPool64;
    InitMemoryPool<128, 10> m_memPool128;
    InitMemoryPool<256, 10> m_memPool256;
    InitMemoryPool<512, 10> m_memPool512;
    InitMemoryPool<1024, 10> m_memPool1024;
    MemoryPool* m_szPool64[1024 + 1];

private:
    MemoryMgr();
    ~MemoryMgr();
    void initMemPool(int nBegin, int nEnd, MemoryPool* pMemPool);
public:
    static MemoryMgr& Instance();
    void* alloctMem(std::size_t nSize);
    void freeMem(void* p);
};

#endif