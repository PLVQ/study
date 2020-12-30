#ifndef _CELL_TASK_H_
#define _CELL_TASK_H_

#include "public_header.h"
// 任务基类
class CellTask
{
public:
    CellTask();
    virtual ~CellTask();
    virtual void doTask();
};

class CellTaskServer
{
private:
    std::list<CellTask *> m_tasks;
    std::list<CellTask *> m_tasksBuf;
    std::mutex m_mutex;

public:
    CellTaskServer();
    virtual ~CellTaskServer();
    // 添加任务
    void addTask(CellTask *pTask);
    // 启动工作线程
    void onStart();

protected:
    // 工作函数
    void onRun();
};

#endif