//
// Created by ryan1 on 10/02/2021.
//

#ifndef SOLIDENGINE_TASK_HPP
#define SOLIDENGINE_TASK_HPP




namespace Solid
{



  /*  class SOLID_API TaskPairInterface
    {
    public:
        TaskPairInterface() =default;
        virtual void operator()() = 0;
        virtual ~TaskPairInterface() =default;

    };
*/


    class SOLID_API TaskPair //: public TaskPairInterface
    {
    public:
        std::function<void()> func;

        template<typename FuncType, typename... Args>
        explicit TaskPair( FuncType&& function,  Args&&... argument)
        {
            func =  std::function<void()>(std::bind(function, argument...));
        };

        TaskPair(TaskPair&& t) noexcept
        {
            func = std::move(t.func);
        }
        TaskPair(const TaskPair& pair)
        {
            func =  std::function<void()>(pair.func);
        }
        TaskPair& operator=(const TaskPair& pair)
        {
            func =  std::function<void()>(pair.func);
            return *this;
        }
        ~TaskPair() /*override*/ = default;

        void operator()() //override
        {
            /**
            * RC:
            *   if(func)
            *       func();
            */

            if(!func)
                return ;
            func();
        }

    };



    enum class SOLID_API ETaskType : int
    {

        RESOURCES_LOADER = 0,
        PHYSICS = 1,
        STANDALONE = 2,
        GENERAL = 3,
    };

    class SOLID_API Task
    {
    public:

    private:
        ETaskType taskType = ETaskType::RESOURCES_LOADER;
        std::string ID = "NO_ID";
        std::atomic_bool bHasRun ;
        std::atomic_bool  bDispatched ;
        std::atomic_bool  bInProgress ;
        std::atomic_bool  bExceptionCatch ;
        std::string_view exceptionBuffer;
        struct ID_Internal
        {
            std::string ID;
        };
    public:

        std::shared_ptr<TaskPair> pairData = nullptr;
        template<typename FuncType, typename... Args>
        explicit Task( FuncType&& _function,  Args&&... _args);
        template<typename FuncType, typename... Args>
        explicit Task(ID_Internal&& _id, ETaskType _level, FuncType&& _function, Args&&... _args);

        template<typename FuncType, typename... Args>
        explicit Task( ID_Internal&& _id, FuncType&& _function,  Args&&... _args);

        template<typename FuncType, typename... Args>
        Task* SetTask( FuncType&& _function,  Args&&... _args);


        Task()
        {
            bHasRun = false;
            bInProgress = false;
            bDispatched = false;
            bExceptionCatch = false;
            pairData = nullptr;
        }
        Task(Task&& _t) noexcept
        {
            taskType = (_t.taskType);
            ID = std::move(_t.ID);
            bHasRun = (_t.bHasRun.load());
            bDispatched = (_t.bDispatched.load());
            bExceptionCatch = (_t.bExceptionCatch.load());
            pairData = _t.pairData;
        }
        Task(const Task& _t) = delete;
        /*{
            Type = (t.Type);
            ID = t.ID;
            ExceptionBuffer = t.ExceptionBuffer;
            bHasRun = (t.bHasRun);
            bDispatched = (t.bDispatched);
            bExceptionCatch = (t.bExceptionCatch);
            PairData = std::make_shared<TaskPair>(*t.PairData);
        }*/
        Task& operator=(const Task& _t) = delete;
        /*{
            Type = (t.Type);
            ID = t.ID;
            ExceptionBuffer = t.ExceptionBuffer;
            bHasRun = (t.bHasRun);
            bDispatched = (t.bDispatched);
            bExceptionCatch = (t.bExceptionCatch);
            PairData = std::make_shared<TaskPair>(*t.PairData);
            return *this;
        }*/
        Task& operator=(Task&& _t)  noexcept
        {
            taskType = (_t.taskType);
            ID = std::move(_t.ID);
            bHasRun = (_t.bHasRun.load());
            bDispatched = (_t.bDispatched.load());
            bExceptionCatch = (_t.bExceptionCatch.load());
            pairData = _t.pairData;
            return *this;
        }

        void operator()()
        {

            try
            {

                if(pairData != nullptr)
                {
                    bInProgress = true;
                    (*pairData)();
                    bInProgress = false;
                    bHasRun = true;
                }
                else
                {
                    throw std::exception ("Task PairData is Nullptr");
                }
            }
            catch(const std::exception& e)
            {

                bExceptionCatch = true;
                bHasRun = true;
                bInProgress = false;
                exceptionBuffer = "[ERROR]: Task ID " + ID + " has encountered an exception. Message:(\"" + e.what() + "\")";

            }
            bHasRun = true;
        }

        Task* SetType(const ETaskType&& p) { taskType = p; return this;}

        [[nodiscard]] ETaskType GetType()  const { return taskType;}
        [[nodiscard]] bool IsFinished()   const { return bHasRun.load();}
        [[nodiscard]] bool IsInProgress() const { return bInProgress.load();}
        [[nodiscard]] bool IsDispatched() const { return bDispatched.load();}
        Task* Dispatch()
        {
            bDispatched = true;
            return this;
        }
        Task* UnDispatch()
        {
            bDispatched = false;
            return this;
        }
        [[nodiscard]] bool Error() const { return bExceptionCatch.load();}
        void ResetRunning() { bHasRun = false; bExceptionCatch = false;}
        static ID_Internal MakeID(const std::string& _sw) { return ID_Internal{_sw};}
        std::string_view getID(){return std::forward<std::string_view>(std::string_view(ID));}
        virtual ~Task(){
            if(pairData != nullptr)
            {
                pairData = nullptr;
            }
        }

    };



    template<typename FuncType, typename... Args>
    Task::Task( FuncType&& _function,  Args&&... _args)
    {
        if(pairData != nullptr)
            pairData = nullptr;
        pairData = std::make_shared<TaskPair>(std::forward<FuncType>(_function), std::forward<Args>(_args)...);
        bHasRun = false;
    }
    template<typename FuncType, typename... Args>
    Task::Task(ID_Internal&& _id, ETaskType _level, FuncType&& _function, Args&&... _args)
    {
        ID = _id.ID;
        taskType = _level;
        if(pairData != nullptr)
            pairData = nullptr;
        pairData = std::make_shared<TaskPair>(std::forward<FuncType>(_function), std::forward<Args>(_args)...);
        bHasRun = false;
    }
    template<typename FuncType, typename... Args>
    Task::Task( ID_Internal&& _id, FuncType&& _function,  Args&&... _args)
    {
        ID = _id.ID;
        if(pairData != nullptr)
            pairData = nullptr;
        pairData = std::make_shared<TaskPair>(std::forward<FuncType>(_function), std::forward<Args>(_args)...);
        bHasRun = false;
    }



    template<typename FuncType, typename... Args>
    Task* Task::SetTask( FuncType&& _function,  Args&&... _args)
    {
        if(pairData != nullptr)
            pairData = nullptr;
        pairData = std::make_shared<TaskPair>(std::forward<FuncType&&>(_function), std::forward<Args>(_args)...);
        bHasRun = false;

        return this;

    }

}
#endif //SOLIDENGINE_TASK_HPP
