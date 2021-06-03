//
// Created by ryan1 on 09/03/2021.
//
#include <functional>
#include <mutex>
#include "Build/solidAPI.hpp"
#include "Core/task.hpp"
#include "Core/taskManager.hpp"

using namespace Solid;
Solid::TaskManager::TaskManager()
{

    taskPool.reserve(50);
}

Task *Solid::TaskManager::getTaskByType(const ETaskType &_s)
{
    std::lock_guard<std::mutex> Lock(taskSystemGuard);
    for(auto& element : taskPool)
    {
        if(element->IsInProgress() || element->Error() || element->IsFinished() || element->IsDispatched())
            continue;
        if(element->GetType() == _s)
        {
            element->Dispatch();
            return element;
        }
    }
    return nullptr;
}

Task *Solid::TaskManager::GetFirstAvailableTask() {
    std::lock_guard<std::mutex> Lock(taskSystemGuard);
    for(auto& element : taskPool)
    {
        if(element->IsInProgress() || element->Error() || element->IsFinished() || element->IsDispatched())
            continue;

        element->Dispatch();
        return element;
    }
    return nullptr;
}

Task *Solid::TaskManager::getTaskByID(const std::string &_s) {
    //std::lock_guard<std::mutex> Lock(TaskSystemGuard);
    for(auto& element : taskPool)
    {

        if(element->getID() == _s)
        {
            return element;
        }
    }
    return nullptr;
}

Task *Solid::TaskManager::getTaskByIndex(int _index) {
    std::lock_guard<std::mutex> Lock(taskSystemGuard);
    if(taskPool.size() - 1 < _index)
    {
        if(taskPool[_index]->IsDispatched())
            return nullptr;
        return (taskPool[_index]);
    }
    return nullptr;
}

TaskManager &Solid::TaskManager::RemoveTaskById(const std::string_view &_s) {
    std::lock_guard<std::mutex> Lock(taskSystemGuard);
    for(int i = 0; i < taskPool.size(); ++i)
    {
        auto& element = taskPool.at(i);
        if(element->getID() == _s)
        {
            if(!element->IsDispatched())
            {
            	delete element;
                taskPool.erase(taskPool.begin() + i);
                break;
            }

        }
    }
    return *this;
}

Task &Solid::TaskManager::AddTask(Task &&_t) {
    std::lock_guard<std::mutex> Lock(taskSystemGuard);
    taskPool.push_back(new Task(std::move(_t)));
    return *taskPool.back();
}

bool Solid::TaskManager::IsEmpty()
{
    std::lock_guard<std::mutex> Lock(taskSystemGuard);
    bool b = true;
    for(auto & element : taskPool)
    {
        if(element->IsFinished() || element->IsInProgress() || element->Error() || element->IsDispatched())
            continue;
        b =false;
    }
    return b;
}

TaskManager::~TaskManager()
{
	std::lock_guard<std::mutex> Lock(taskSystemGuard);
	for(auto & element : taskPool)
	{
		delete element;
	}

}
