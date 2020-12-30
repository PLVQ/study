#include "memory_mgr.h"

MemoryBlock::MemoryBlock(/* args */)
{
}

MemoryBlock::~MemoryBlock()
{
}

MemoryPool::MemoryPool(/* args */)
{
    m_nBlockSize = 0;
    m_nBlockCount = 0;
    m_pBuf = nullptr;
    m_pHeader = nullptr;
}

MemoryPool::~MemoryPool()
{
}

void MemoryPool::initMemPool()
{
    assert(nullptr == m_pBuf);
    size_t realBlockSize = sizeof(MemoryBlock) + m_nBlockSize;
    size_t bufSize = m_nBlockCount * realBlockSize;
    m_pBuf = (char *)malloc(bufSize);
    m_pHeader = (MemoryBlock *)m_pBuf;
    m_pHeader->nId = 0;
    m_pHeader->nRet = 0;
    m_pHeader->bPool = true;
    m_pHeader->pNext = nullptr;
    m_pHeader->pPool = this;

    MemoryBlock *pBlock = m_pHeader;
    for (int i = 1; i < m_nBlockCount; i++)
    {
        MemoryBlock *pTemp = (MemoryBlock *)(m_pBuf + realBlockSize * i);
        pTemp->nId = 0;
        pTemp->nRet = 0;
        pTemp->bPool = true;
        pTemp->pNext = nullptr;
        pTemp->pPool = this;

        pBlock->pNext = pTemp;
        pBlock = pTemp;
    }
}

void* MemoryPool::alloctMem(size_t nSize)
{
    if(!m_pBuf)
    {
        initMemPool();
    }

    MemoryBlock* pReturn = nullptr;
    if(nullptr == m_pHeader)
    {
        pReturn = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
        pReturn->bPool = false;
        pReturn->nId = -1;
        pReturn->nRet = 0;
        pReturn->pPool = nullptr;
        pReturn->pNext = nullptr;
    }
    else
    {
        pReturn = m_pHeader;
        m_pHeader = m_pHeader->pNext;
        assert(0 == pReturn->nRet);
        pReturn->nRet = 1;
    }

    std::cout << "alloctMem<" << pReturn << "> id = " << pReturn->nId << " size = " << nSize << std::endl;
    return ((char*)pReturn + sizeof(MemoryBlock));
}

void  MemoryPool::freeMem(void* pMem)
{
    MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
    assert(1 == pBlock->nRet);
    if(--pBlock->nRet != 0)
    {
        return;
    }
    if(pBlock->bPool)
    {
        pBlock->pNext = m_pHeader;
        m_pHeader = pBlock;
    }
    else
    {
        free(pBlock);
    }

}

MemoryMgr::MemoryMgr()
{
    initMemPool(0, 64, &m_memPool64);
    initMemPool(65, 128, &m_memPool64);
    initMemPool(129, 256, &m_memPool64);
    initMemPool(257, 512, &m_memPool64);
    initMemPool(513, 1024, &m_memPool64);
}

MemoryMgr::~MemoryMgr()
{
}

void MemoryMgr::initMemPool(int nBegin, int nEnd, MemoryPool* pMemPool)
{
    for(int i = nBegin; i <= nEnd; ++i)
    {
        m_szPool64[i] = pMemPool;
    }
}

MemoryMgr &MemoryMgr::Instance()
{
    static MemoryMgr memMgr;
    return memMgr;
}

void *MemoryMgr::alloctMem(std::size_t nSize)
{
    if (nSize <= 64)
    {
        return m_szPool64[nSize]->alloctMem(nSize);
    }
    else
    {
        MemoryBlock* pReturn = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
        pReturn->bPool = false;
        pReturn->nId = -1;
        pReturn->nRet = 0;
        pReturn->pPool = nullptr;
        pReturn->pNext = nullptr;
        std::cout << "alloctMem<" << pReturn << "> id = " << pReturn->nId << " size = " << nSize << std::endl;
        return ((char*)pReturn + sizeof(MemoryBlock));
    }
}

void MemoryMgr::freeMem(void* pMem)
{
    MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
    std::cout << "alloctMem<" << pBlock << "> id = " << pBlock->nId << std::endl;
    if(pBlock->bPool)
    {
        pBlock->pPool->freeMem(pMem);
    }
    else
    {
        free(pBlock);
    }
}