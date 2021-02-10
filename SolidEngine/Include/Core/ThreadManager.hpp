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

        TaskSystem* TaskManager = nullptr;
        bool bTerminateThread = false;
        bool bPauseThread = true;
        bool bWaitForThread = false;
        void StopWaitForThread(std::mutex& InternalDataMutex)
        {
            std::lock_guard<std::mutex> Lock(InternalDataMutex);
            bWaitForThread = false;

        }
        int ID = 0;
    };


    void RunThread(class Thread* self, T_Internal* self_Internal, std::mutex& selfInternalMutex);
    class Thread
    {
        std::mutex* internalDataMutex;
        TaskType ThreadType = TaskType::GENERAL;
        std::thread* thread = nullptr;
        T_Internal internal;
    public:


        Thread()
        {
            internalDataMutex = new std::mutex();
            thread = new std::thread(RunThread, this, &(this->internal), std::ref(*internalDataMutex));
        }
        Thread(Thread& t) = delete;
        /*{
            internalDataMutex = new std::mutex();
            internal = t.internal;
            ThreadType = t.ThreadType;
            thread = new std::thread(RunThread, this, &(this->internal), std::ref(*internalDataMutex));
        }*/
        Thread(int* i)
        {
            internalDataMutex = new std::mutex();
            thread = new std::thread(RunThread, this, &(this->internal), std::ref(*internalDataMutex));
            internal.ID = *i;
            *i -=1;
        }
        ~Thread()
        {
            thread->join();
            delete thread;
        }


        void Play()
        {
            std::lock_guard<std::mutex> Lock(*internalDataMutex);
            internal.bPauseThread = false;
        }
        void Pause()
        {
            std::lock_guard<std::mutex> Lock(*internalDataMutex);
            internal.bPauseThread = true;
        }

        [[nodiscard]] bool     IsTerminating()
        {
            std::lock_guard<std::mutex> Lock(*internalDataMutex);
            return internal.bTerminateThread;
        }
        [[nodiscard]] bool     IsPaused()
        {
            std::lock_guard<std::mutex> Lock(*internalDataMutex);
            return internal.bPauseThread;
        }
        void     Terminate()
        {
            std::lock_guard<std::mutex> Lock(*internalDataMutex);
            internal.bTerminateThread = true;
        }
        [[nodiscard]] TaskType GetThreadType()
        {
            return ThreadType ;
        }
        void SetThreadType(TaskType t)
        {
            ThreadType = t;
        }

        void join()
        {

            {
                std::lock_guard<std::mutex> Lock(*internalDataMutex);
                internal.bWaitForThread = true;
            }
            bool Wait = true;
            while (Wait)
            {
                {
                    std::lock_guard<std::mutex> Lock(*internalDataMutex);
                    Wait = internal.bWaitForThread;
                }
                {
                    std::this_thread::yield();
                }
            }
        }
        void join(TerminateAfterJoin t)
        {
            {
                std::lock_guard<std::mutex> Lock(*internalDataMutex);
                internal.bWaitForThread = true;
                internal.bTerminateThread = true;
            }
            bool Wait = true;
            while (Wait)
            {
                {
                    std::lock_guard<std::mutex> Lock(*internalDataMutex);
                    Wait = internal.bWaitForThread;
                }
                {
                    std::this_thread::yield();
                }
            }
        }


    };



    class [[maybe_unused]] ThreadManager
    {
        uint8_t MaxNumThread ;
        uint8_t MaxStandAloneThread ;
        std::vector<Thread*> ThreadPool ;
    public:
        ThreadManager();
        ~ThreadManager() = default;
        [[maybe_unused]] ThreadManager& PlayAllThreads();
        [[maybe_unused]] ThreadManager& joinAllThread();
        [[maybe_unused]] ThreadManager& joinAllThread(TerminateAfterJoin);
        [[maybe_unused]] void           TerminateAllThreads();
        [[maybe_unused]] uint8_t        GetMaxNumThread()        const {return MaxNumThread;}
        [[maybe_unused]] uint8_t        GetMaxStandAloneThread() const {return MaxStandAloneThread;}
        [[maybe_unused]] ThreadManager& SetMaxStandAloneThread(uint8_t n)
        {
            MaxStandAloneThread = n;
            if(MaxStandAloneThread > MaxNumThread)
            {
                MaxStandAloneThread = MaxNumThread;
            }
            else if(MaxStandAloneThread < 0)
            {
                MaxStandAloneThread = 0;
            }
            return *this;
        }
    };

}
#endif //SOLIDENGINE_THREADMANAGER_HPP
