//
// Created by ryan1 on 10/02/2021.
//

#ifndef SOLIDENGINE_TASKMANAGER_HPP
#define SOLIDENGINE_TASKMANAGER_HPP


namespace Solid
{


    class SOLID_API TaskManager
    {
        std::mutex taskSystemGuard;
        std::vector<Task> taskPool;
    public:

        
        TaskManager();

        TaskManager(TaskManager&) = delete;
        Task* getTaskByType(const ETaskType& _s);
        Task* GetFirstAvailableTask();
        Task* getTaskByID(const std::string_view& _s);
        Task* getTaskByIndex(int _index);
        Task& AddTask(Task&& _t);
        TaskManager& RemoveTaskById(const std::string_view& _s);
        bool IsEmpty();

    };

}



#endif //SOLIDENGINE_TASKMANAGER_HPP
