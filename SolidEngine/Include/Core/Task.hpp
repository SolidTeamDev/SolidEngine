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
            if(!func)
                return ;
            func();
        }

    };



    enum class SOLID_API TaskType : int
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
        TaskType Type = TaskType::RESOURCES_LOADER;
        std::string ID = "NO_ID";
        std::atomic_bool bHasRun ;
        std::atomic_bool  bDispatched ;
        std::atomic_bool  bInProgress ;
        std::atomic_bool  bExceptionCatch ;
        std::string_view ExceptionBuffer;
        struct ID_Internal
        {
            std::string ID;
        };
    public:

        std::shared_ptr<TaskPair> PairData = nullptr;
        template<typename FuncType, typename... Args>
        explicit Task( FuncType&& function,  Args&&... args);
        template<typename FuncType, typename... Args>
        explicit Task(ID_Internal&& id, TaskType level, FuncType&& function, Args&&... args);

        template<typename FuncType, typename... Args>
        explicit Task( ID_Internal&& id, FuncType&& function,  Args&&... args);

        template<typename FuncType, typename... Args>
        Task* SetTask( FuncType&& function,  Args&&... args);


        Task()
        {
            bHasRun = false;
            bInProgress = false;
            bDispatched = false;
            bExceptionCatch = false;
            PairData = nullptr;
        }
        Task(Task&& t) noexcept
        {
            Type = (t.Type);
            ID = std::move(t.ID);
            bHasRun = (t.bHasRun.load());
            bDispatched = (t.bDispatched.load());
            bExceptionCatch = (t.bExceptionCatch.load());
            PairData = t.PairData;
        }
        Task(const Task& t) = delete;
        /*{
            Type = (t.Type);
            ID = t.ID;
            ExceptionBuffer = t.ExceptionBuffer;
            bHasRun = (t.bHasRun);
            bDispatched = (t.bDispatched);
            bExceptionCatch = (t.bExceptionCatch);
            PairData = std::make_shared<TaskPair>(*t.PairData);
        }*/
        Task& operator=(const Task& t) = delete;
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
        Task& operator=(Task&& t)  noexcept
        {
            Type = (t.Type);
            ID = std::move(t.ID);
            bHasRun = (t.bHasRun.load());
            bDispatched = (t.bDispatched.load());
            bExceptionCatch = (t.bExceptionCatch.load());
            PairData = t.PairData;
            return *this;
        }

        void operator()()
        {

            try
            {

                if(PairData != nullptr)
                {
                    bInProgress = true;
                    (*PairData)();
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
                ExceptionBuffer = "[ERROR]: Task ID " + ID +" has encountered an exception. Message:(\"" + e.what() +"\")";

            }
            bHasRun = true;
        }

        Task* SetType(const TaskType&& p) { Type = p; return this;}

        [[nodiscard]] TaskType GetType()  const { return Type;}
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
        static ID_Internal MakeID(const std::string& sw) { return ID_Internal{sw};}
        std::string_view getID(){return std::forward<std::string_view>(std::string_view(ID));}
        virtual ~Task(){
            if(PairData != nullptr)
            {
                PairData = nullptr;
            }
        }

    };



    template<typename FuncType, typename... Args>
    Task::Task( FuncType&& function,  Args&&... args)
    {
        if(PairData != nullptr)
            PairData = nullptr;
        PairData = std::make_shared<TaskPair>(std::forward<FuncType>(function), std::forward<Args>(args)...);
        bHasRun = false;
    }
    template<typename FuncType, typename... Args>
    Task::Task(ID_Internal&& id, TaskType level, FuncType&& function, Args&&... args)
    {
        ID = id.ID;
        Type = level;
        if(PairData != nullptr)
            PairData = nullptr;
        PairData = std::make_shared<TaskPair>(std::forward<FuncType>(function), std::forward<Args>(args)...);
        bHasRun = false;
    }
    template<typename FuncType, typename... Args>
    Task::Task( ID_Internal&& id, FuncType&& function,  Args&&... args)
    {
        ID = id.ID;
        if(PairData != nullptr)
            PairData = nullptr;
        PairData = std::make_shared<TaskPair>(std::forward<FuncType>(function), std::forward<Args>(args)...);
        bHasRun = false;
    }



    template<typename FuncType, typename... Args>
    Task* Task::SetTask( FuncType&& function,  Args&&... args)
    {
        if(PairData != nullptr)
            PairData = nullptr;
        PairData = std::make_shared<TaskPair>(std::forward<FuncType&&>(function), std::forward<Args>(args)...);
        bHasRun = false;

        return this;

    }

}
#endif //SOLIDENGINE_TASK_HPP
