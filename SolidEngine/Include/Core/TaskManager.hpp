//
// Created by ryan1 on 10/02/2021.
//

#ifndef SOLIDENGINE_TASKMANAGER_HPP
#define SOLIDENGINE_TASKMANAGER_HPP


namespace Solid
{

    class SOLID_API TaskManager {
        std::mutex taskSystemGuard;
        std::vector<Task> taskPool;
    public:

        TaskManager()
        {

            taskPool.reserve(50);
        }

        TaskManager(TaskManager&) = delete;
        Task* getTaskByType(const ETaskType& _s)
        {
            std::lock_guard<std::mutex> Lock(taskSystemGuard);
            for(auto& element : taskPool)
            {
                if(element.IsInProgress() || element.Error() || element.IsFinished() || element.IsDispatched())
                    continue;
                if(element.GetType() == _s)
                {
                    element.Dispatch();
                    return &element;
                }
            }
            return nullptr;
        }
        Task* GetFirstAvailableTask()
        {
            std::lock_guard<std::mutex> Lock(taskSystemGuard);
            for(auto& element : taskPool)
            {
                if(element.IsInProgress() || element.Error() || element.IsFinished() || element.IsDispatched())
                    continue;

                element.Dispatch();
                return &element;
            }
            return nullptr;
        }
        Task* getTaskByID(const std::string_view& _s)
        {
            //std::lock_guard<std::mutex> Lock(TaskSystemGuard);
            for(auto& element : taskPool)
            {

                if(element.getID() == _s)
                {
                    return &element;
                }
            }
            return nullptr;
        }
        Task* getTaskByIndex(int _index)
        {
            std::lock_guard<std::mutex> Lock(taskSystemGuard);
            if(taskPool.size() - 1 < _index)
            {
                if(taskPool[_index].IsDispatched())
                    return nullptr;
                return &(taskPool[_index]);
            }
            return nullptr;
        }
        Task& AddTask(Task&& _t)
        {
            std::lock_guard<std::mutex> Lock(taskSystemGuard);
            taskPool.push_back(std::move(_t));
            return taskPool.back();
        }
        TaskManager& RemoveTaskById(const std::string_view& _s)
        {
            std::lock_guard<std::mutex> Lock(taskSystemGuard);
            for(int i = 0; i < taskPool.size(); ++i)
            {
                auto& element = taskPool.at(i);
                if(element.getID() == _s)
                {
                    if(!element.IsDispatched())
                    {
                        taskPool.erase(taskPool.begin() + i);
                        break;
                    }

                }
            }
            return *this;
        }
        bool IsEmpty()
        {
            std::lock_guard<std::mutex> Lock(taskSystemGuard);
            bool b = true;
            for(auto & element : taskPool)
            {
                if(element.IsFinished() || element.IsInProgress() || element.Error() || element.IsDispatched())
                    continue;
                b =false;
            }
            return b;
        }

    };

}


#endif //SOLIDENGINE_TASKMANAGER_HPP
