//
// Created by ryan1 on 10/02/2021.
//
//
// Created by ryan1 on 26/11/2020.
//

#include "Core/olidMultiThread.hpp"
namespace Solid
{

    void RunThread(struct Thread *_self, T_Internal* _self_Internal, std::mutex& _selfInternalMutex)
    {
        if(_self == nullptr)
            return;


        bool terminate;
        bool isPaused ;
        {
            std::lock_guard<std::mutex> Lock(_selfInternalMutex);
            terminate = _self_Internal->bTerminateThread;
            isPaused = _self_Internal->bPauseThread;
        }

        while (!(terminate))
        {

            while (isPaused)
            {
                {
                    std::lock_guard<std::mutex> Lock(_selfInternalMutex);
                    isPaused = _self_Internal->bPauseThread;
                    if(_self_Internal->bWaitForThread)
                        _self_Internal->bWaitForThread = false;
                }
                std::this_thread::sleep_for(std::chrono::microseconds (100));
                std::this_thread::yield();

            }
            {
                std::lock_guard<std::mutex> Lock(_selfInternalMutex);

                TaskManager* manager = _self_Internal->taskManager;
                if(manager == nullptr)
                {
                    std::this_thread::sleep_for(std::chrono::microseconds (10));
                    std::this_thread::yield();
                    continue;
                }
                Task* task = manager->getTaskByType(_self->GetThreadType());
                if(task == nullptr)
                {
                    task = manager->GetFirstAvailableTask();


                }
                if(task== nullptr)
                {
                    if(_self_Internal->bWaitForThread)
                        _self_Internal->bWaitForThread = false;
                    std::this_thread::sleep_for(std::chrono::microseconds (10));
                    std::this_thread::yield();

                }
                else
                {
                    task->operator()();
                    task->UnDispatch();
                    if(_self_Internal->bWaitForThread)
                        _self_Internal->bWaitForThread = false;
                }

            }
            {
                std::lock_guard<std::mutex> Lock(_selfInternalMutex);
                terminate = _self_Internal->bTerminateThread;
                isPaused = _self_Internal->bPauseThread;
            }
        }
       ///Terminate thread

        _self_Internal->StopWaitForThread(_selfInternalMutex);
    }

    ThreadManager::ThreadManager(TaskManager* Manager)
    {
        maxNumThread = std::thread::hardware_concurrency();
        int i = maxNumThread;
        threadPool.reserve(maxNumThread);
        for (int j = 0; j < maxNumThread; ++j) {
            threadPool.emplace_back(new Thread(Manager, &i));
        }





    }



    ThreadManager &ThreadManager::PlayAllThreads()
    {
        for (Thread* thread : threadPool) {
            thread->Play();
        }
        return *this;
    }
    ThreadManager &ThreadManager::PauseAllThreads()
    {
        for (Thread* thread : threadPool) {
            thread->Pause();
        }
        return *this;
    }
    ThreadManager &ThreadManager::joinAllThread() {

        for (Thread* thread : threadPool) {
            thread->join();
        }
        return *this;
    }



    ThreadManager &ThreadManager::joinAllThread(TerminateAfterJoin t) {

        for (Thread* thread : threadPool) {
            thread->join(t);
        }
        return *this;
    }

    void ThreadManager::TerminateAllThreads()
    {
        for(int i = 0; i < maxNumThread; ++i)
        {
            Thread* element = threadPool[i];
            element->join( TerminateAfterJoin{});
        }
    }
    
}
