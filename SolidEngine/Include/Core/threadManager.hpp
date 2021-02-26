//
// Created by ryan1 on 10/02/2021.
//

#ifndef SOLIDENGINE_THREADMANAGER_HPP
#define SOLIDENGINE_THREADMANAGER_HPP


namespace Solid
{
   struct TerminateAfterJoin{};
    struct NoExec{};
    struct T_Internal
    {

        TaskManager* taskManager = nullptr;
        bool bTerminateThread = false;
        bool bPauseThread = true;
        bool bWaitForThread = false;
        void StopWaitForThread(std::mutex& _InternalDataMutex)
        {
            std::lock_guard<std::mutex> Lock(_InternalDataMutex);
            bWaitForThread = false;

        }
        int ID = 0;
    };


    void SOLID_API RunThread(struct Thread *_self, T_Internal* _self_Internal, std::mutex& _selfInternalMutex);
    class SOLID_API Thread
    {
        std::mutex internalDataMutex;
        ETaskType threadType = ETaskType::GENERAL;
        std::thread* thread = nullptr;
        T_Internal internal;
    public:


        Thread()
        {
            //internalDataMutex = new std::mutex();
            thread = new std::thread(RunThread, this, &(this->internal), std::ref(internalDataMutex));
        }
        Thread(TaskManager* _m)
        {
            //internalDataMutex = new std::mutex();
            internal.taskManager = _m;
            thread = new std::thread(RunThread, this, &(this->internal), std::ref(internalDataMutex));
        }
        Thread(Thread& t) = delete;

        Thread(TaskManager* _m, int* _i)
        {
            //internalDataMutex = new std::mutex();
            internal.taskManager = _m;
            internal.ID = *_i;
            thread = new std::thread(RunThread, this, &(this->internal), std::ref(internalDataMutex));


            *_i -=1;
        }
        ~Thread()
        {
            thread->join();
            delete thread;
            //delete internalDataMutex;
        }
        void SetManager(TaskManager* m)
        {
            internal.taskManager = m;
        }


        void Play()
        {
            std::lock_guard<std::mutex> Lock(internalDataMutex);
            internal.bPauseThread = false;
        }
        void Pause()
        {
            std::lock_guard<std::mutex> Lock(internalDataMutex);
            internal.bPauseThread = true;
        }

        [[nodiscard]] bool     IsTerminating()
        {
            std::lock_guard<std::mutex> Lock(internalDataMutex);
            return internal.bTerminateThread;
        }
        [[nodiscard]] bool     IsPaused()
        {
            std::lock_guard<std::mutex> Lock(internalDataMutex);
            return internal.bPauseThread;
        }
        void     Terminate()
        {
            std::lock_guard<std::mutex> Lock(internalDataMutex);
            internal.bTerminateThread = true;
        }
        [[nodiscard]] ETaskType GetThreadType()
        {
            return threadType ;
        }
        void SetThreadType(ETaskType t)
        {
            threadType = t;
        }

        void join()
        {

            {
                std::lock_guard<std::mutex> Lock(internalDataMutex);
                internal.bWaitForThread = true;
            }
            bool Wait = true;
            while (Wait)
            {

                {
                    std::this_thread::yield();
                }
                {
                    std::lock_guard<std::mutex> Lock(internalDataMutex);
                    Wait = internal.bWaitForThread;
                }
            }
        }
        void join(TerminateAfterJoin _t)
        {
            {
                std::lock_guard<std::mutex> Lock(internalDataMutex);
                internal.bWaitForThread = true;
                internal.bTerminateThread = true;
            }
            bool Wait = true;
            while (Wait)
            {
                {
                    std::lock_guard<std::mutex> Lock(internalDataMutex);
                    Wait = internal.bWaitForThread;
                }
                {
                    std::this_thread::yield();
                }
            }
        }


    };



    class SOLID_API ThreadManager
    {
        uint8_t maxNumThread ;
        uint8_t maxStandAloneThread ;
        std::vector<Thread*> threadPool ;
    public:
        ThreadManager(TaskManager* _Manager);
        ~ThreadManager() = default;
        [[maybe_unused]] ThreadManager& PlayAllThreads();
        [[maybe_unused]] ThreadManager& PauseAllThreads();
        [[maybe_unused]] ThreadManager& joinAllThread();
        [[maybe_unused]] ThreadManager& joinAllThread(TerminateAfterJoin);
        [[maybe_unused]] void           TerminateAllThreads();
        [[maybe_unused]] uint8_t        GetMaxNumThread()        const {return maxNumThread;}
        [[maybe_unused]] uint8_t        GetMaxStandAloneThread() const {return maxStandAloneThread;}
        [[maybe_unused]] ThreadManager& SetMaxStandAloneThread(uint8_t _n)
        {
            maxStandAloneThread = _n;
            if(maxStandAloneThread > maxNumThread)
            {
                maxStandAloneThread = maxNumThread;
            }
            else if(maxStandAloneThread < 0)
            {
                maxStandAloneThread = 0;
            }
            return *this;
        }
    };

}
#endif //SOLIDENGINE_THREADMANAGER_HPP
