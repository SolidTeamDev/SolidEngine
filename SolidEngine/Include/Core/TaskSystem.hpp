//
// Created by ryan1 on 10/02/2021.
//

#ifndef SOLIDENGINE_TASKSYSTEM_HPP
#define SOLIDENGINE_TASKSYSTEM_HPP


namespace Solid
{

    class TaskSystem {
        std::mutex TaskSystemGuard;
        std::vector<Task> TaskPool;
    public:

        TaskSystem()
        {
            TaskPool.reserve(50);
        }
        Task* getTaskByType(const TaskType& s)
        {
            std::lock_guard<std::mutex> Lock(TaskSystemGuard);
            for(auto& element : TaskPool)
            {
                if(element.IsInProgress() || element.Error() || element.IsFinished() || element.IsDispatched())
                    continue;
                if(element.GetType() == s)
                {
                    element.Dispatch();
                    return &element;
                }
            }
            return nullptr;
        }
        Task* GetFirstAvailableTask()
        {
            std::lock_guard<std::mutex> Lock(TaskSystemGuard);
            for(auto& element : TaskPool)
            {
                if(element.IsInProgress() || element.Error() || element.IsFinished() || element.IsDispatched())
                    continue;

                element.Dispatch();
                return &element;
            }
            return nullptr;
        }
        Task* getTaskByID(const std::string_view& s)
        {
            std::lock_guard<std::mutex> Lock(TaskSystemGuard);
            for(auto& element : TaskPool)
            {

                if(element.getID() == s)
                {
                    if(element.IsDispatched())
                        return nullptr;
                    return &element;
                }
            }
            return nullptr;
        }
        Task* getTaskByIndex(int index)
        {
            std::lock_guard<std::mutex> Lock(TaskSystemGuard);
            if(TaskPool.size()-1 < index)
            {
                if(TaskPool[index].IsDispatched())
                    return nullptr;
                return &(TaskPool[index]);
            }
            return nullptr;
        }
        TaskSystem& AddTask(Task&& t)
        {
            std::lock_guard<std::mutex> Lock(TaskSystemGuard);
            TaskPool.push_back(std::move(t));
            return *this;
        }
        TaskSystem& RemoveTaskById(const std::string_view& s)
        {
            std::lock_guard<std::mutex> Lock(TaskSystemGuard);
            for(int i = 0; i < TaskPool.size(); ++i)
            {
                auto& element = TaskPool.at(i);
                if(element.getID() == s)
                {
                    if(!element.IsDispatched())
                    {
                        TaskPool.erase(TaskPool.begin() + i);
                        break;
                    }

                }
            }
            return *this;
        }
        bool IsEmpty()
        {
            std::lock_guard<std::mutex> Lock(TaskSystemGuard);
            bool b = true;
            for(auto & element : TaskPool)
            {
                if(element.IsFinished() || element.IsInProgress() || element.Error() || element.IsDispatched())
                    continue;
                b =false;
            }
            return b;
        }

    };

}


#endif //SOLIDENGINE_TASKSYSTEM_HPP
