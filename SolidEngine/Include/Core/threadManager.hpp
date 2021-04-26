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


        Thread();
        Thread(TaskManager* _m);
        Thread(Thread& t) = delete;

        Thread(TaskManager* _m, int* _i);
        ~Thread();
        void SetManager(TaskManager* m);


        void Play();
        void Pause();

        bool     IsTerminating();
        bool     IsPaused();
        void     Terminate();
        ETaskType GetThreadType();
        void SetThreadType(ETaskType t);

        void join();
        void join(TerminateAfterJoin _t);


    };



    class SOLID_API ThreadManager
    {
        uint8_t maxNumThread ;
        uint8_t maxStandAloneThread ;
        std::vector<Thread*> threadPool ;
    public:
        ThreadManager(TaskManager* _Manager);
        ~ThreadManager() = default;
        ThreadManager& PlayAllThreads();
        ThreadManager& PauseAllThreads();
        ThreadManager& joinAllThread();
        ThreadManager& joinAllThread(TerminateAfterJoin);
        void           TerminateAllThreads();
        uint8_t        GetMaxNumThread()        const {return maxNumThread;}
        uint8_t        GetMaxStandAloneThread() const {return maxStandAloneThread;}
        ThreadManager& SetMaxStandAloneThread(uint8_t _n)
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
