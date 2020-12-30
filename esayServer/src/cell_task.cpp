#include "cell_task.h"

CellTask::CellTask()
{

}

CellTask::~CellTask()
{

}

void CellTask::doTask()
{

}

CellTaskServer::CellTaskServer()
{

}

CellTaskServer::~CellTaskServer()
{

}

void CellTaskServer::onStart()
{
    std::thread t(std::mem_fn(&CellTaskServer::onRun), this);
    t.detach();
}

void CellTaskServer::addTask(CellTask* pTask)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_tasksBuf.push_back(pTask);
}

void CellTaskServer::onRun()
{
    while(true)
    {
        if(!m_tasksBuf.empty())
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            for (auto pTask : m_tasksBuf)
            {
                m_tasks.push_back(pTask);
            }
            m_tasksBuf.clear();
        }

        if(m_tasks.empty())
        {
            std::chrono::milliseconds t(1);
            std::this_thread::sleep_for(t);
            continue;
        }

        //处理任务
        for (auto pTask : m_tasks)
        {
            pTask->doTask();
            delete pTask;
        }

        m_tasks.clear();
    }
}

