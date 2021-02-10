//
// Created by ryan1 on 10/02/2021.
//
//
// Created by ryan1 on 26/11/2020.
//

#include "Core/SolidMultiThread.hpp"
namespace Solid
{

    void RunThread(Thread* self, T_Internal* self_Internal, std::mutex& selfInternalMutex)
    {
        if(self == nullptr)
            return;


        bool terminate;
        bool isPaused ;
        {
            std::lock_guard<std::mutex> Lock(selfInternalMutex);
            terminate = self_Internal->bTerminateThread;
            isPaused = self_Internal->bPauseThread;
        }

        while (!(terminate))
        {

            while (isPaused)
            {
                {
                    std::lock_guard<std::mutex> Lock(selfInternalMutex);
                    isPaused = self_Internal->bPauseThread;
                    if(self_Internal->bWaitForThread)
                        self_Internal->bWaitForThread = false;
                }
                std::this_thread::yield();

            }
            {
                std::lock_guard<std::mutex> Lock(selfInternalMutex);
                TaskSystem* manager = self_Internal->TaskManager;
                if(manager == nullptr)
                {
                    std::this_thread::yield();
                    continue;
                }
                Task* task = manager->getTaskByType(self->GetThreadType());
                if(task == nullptr)
                {
                    task = manager->GetFirstAvailableTask();
                    if(task== nullptr)
                    {
                        std::this_thread::yield();
                        continue;
                    }

                }
                task->operator()();
                task->UnDispatch();
                if(self_Internal->bWaitForThread)
                    self_Internal->bWaitForThread = false;
            }
            {
                std::lock_guard<std::mutex> Lock(selfInternalMutex);
                terminate = self_Internal->bTerminateThread;
                isPaused = self_Internal->bPauseThread;
            }
        }
        //std::cout << "Terminating" << std::endl;

        self_Internal->StopWaitForThread(selfInternalMutex);

    }

    ThreadManager::ThreadManager()
    {
        MaxNumThread = std::thread::hardware_concurrency();
        int i = MaxNumThread;
        ThreadPool.reserve(MaxNumThread);
        for (int j = 0; j < MaxNumThread; ++j) {
            ThreadPool.emplace_back(new Thread(&i));
        }





    }



    ThreadManager &ThreadManager::PlayAllThreads()
    {
        for (Thread* thread : ThreadPool) {
            thread->Play();
        }
        return *this;
    }

    ThreadManager &ThreadManager::joinAllThread() {

        for (Thread* thread : ThreadPool) {
            thread->join();
        }
        return *this;
    }



    ThreadManager &ThreadManager::joinAllThread(TerminateAfterJoin t) {

        for (Thread* thread : ThreadPool) {
            thread->join(t);
        }
        return *this;
    }

    void ThreadManager::TerminateAllThreads()
    {
        for(int i = 0; i <MaxNumThread; ++i)
        {
            Thread* element = ThreadPool[i];
            element->join( TerminateAfterJoin{});
        }
    }
    
}
