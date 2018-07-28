//
// Created by Ethan Breit on 2017-08-12.
//

#include <string>
#include <ge/runtime/Runtime.h>
#include <ge/console/ConsoleIO.h>
#include <ge/memory/GlobalMemory.h>
#include <ge/console/Log.h>

namespace ge
{
	const std::string LOG_TAG = "Runtime";


    typedef std::chrono::nanoseconds    nanoseconds;
    typedef std::chrono::seconds        seconds;
    Runtime::Runtime(std::string name, uint32_t cyclesPerSecond)
    {

        for ( auto g : groups )
            g = nullptr;
        this->name = name;
        this->cyclesPerSecond = cyclesPerSecond;

        shouldRun = true;
    }

	Runtime::Runtime(std::string name)
	{
		for (auto g : groups)
			g = nullptr;
		this->name = name;
		this->cyclesPerSecond = 0;
	}

	void Runtime::run(Runtime* context)
    {
        ///Define Cycle Timing Data
		nanoseconds delta;
		nanoseconds managerDelta;
		std::chrono::steady_clock::time_point currentTime;

		std::chrono::steady_clock::time_point nextCycle;
		std::chrono::steady_clock::time_point nextManage;

		if (context->cyclesPerSecond > 0)
		{
			//Cycle timing
			delta = nanoseconds(nanoseconds(seconds(1)).count() / context->cyclesPerSecond); ///Calculate Delta
			nextCycle = std::chrono::steady_clock::time_point(std::chrono::steady_clock::now()); ///Time to next cycle

			//Cycle Manager Timing
			managerDelta = nanoseconds(nanoseconds(std::chrono::seconds(1)));
			nextManage = std::chrono::steady_clock::time_point(std::chrono::steady_clock::now());
		}
		else
		{
			//Cycle timing
			delta = nanoseconds(0);
			nextCycle = std::chrono::steady_clock::time_point(std::chrono::steady_clock::now()); 

			//Cycle Manager Timing
			managerDelta = nanoseconds(nanoseconds(std::chrono::seconds(1)));
			nextManage = std::chrono::steady_clock::time_point(std::chrono::steady_clock::now());
		}

		nextCycle += (delta);
		nextManage += managerDelta;


        Log::scc(LOG_TAG, "Starting runtime \""+context->name+"\" at " + (context->cyclesPerSecond>0 ? std::to_string(context->cyclesPerSecond) : "unlocked")+" cycles per second.\n");

        while (context->shouldRun)
        {

            currentTime = std::chrono::steady_clock::now();



            if(currentTime>=nextCycle)
            {
                std::chrono::steady_clock::time_point uStart = std::chrono::steady_clock::now();

                nextCycle =  currentTime;
                nextCycle += delta;
                context->cyclesSinceLastClear++;
                context->cyclesSinceLastManage++;

                context->cycle(); /// Cycle Groups;

                std::chrono::steady_clock::time_point uEnd = std::chrono::steady_clock::now();

                std::chrono::duration<float, std::milli> currentDelta(uEnd - uStart);

                context->miliLastDelta  = currentDelta.count();
                context->tempMiliValue  += currentDelta.count();

                //Time calculations
                context->smoothTime += (currentDelta.count()/1000)*3.1415926536;
            }

            if(currentTime>=nextManage)
            {

                context->managesSinceLastClear++;

				Log::tVrb(LOG_TAG, context->name+": "+std::to_string(context->cyclesSinceLastManage)+"\n");

                nextManage =  currentTime;
                nextManage += managerDelta;

                ///Correct Delta

                if(!context->cyclesSinceLastManage*context->managesSinceLastClear==context->cyclesSinceLastClear)
                {
                    uint64_t incorrect = context->cyclesSinceLastManage*context->managesSinceLastClear;
                    uint64_t correct   = context->cyclesSinceLastClear;

                    uint32_t difference;

                    difference = (uint32_t) (correct - incorrect); /// get the discrepancy

                    //delta = nanoseconds(nanoseconds(seconds(1)).count()/(context->cyclesPerSecond+difference)); ///recalculate Delta (to account for the discrepancy)

                }
                else
                {
                    //delta = nanoseconds(nanoseconds(seconds(1)).count()/context->cyclesPerSecond); ///recalculate Delta
                }

                context->lastManagesCycles = context->cyclesSinceLastManage;
                context->lastManagesAverageDelta = context->tempMiliValue/context->cyclesSinceLastManage;
                context->tempMiliValue = 0;

                ///Correct Delta

                context->cyclesSinceLastManage = 0;
                if(context->managesSinceLastClear+1==context->managesBetweenClear) ///check if there has been enough manages to clear the since last clear data. (+1 because counting starts at 0)
                {

                    context->managesSinceLastClear = 0;

                    context->cyclesSinceLastClear  = 0;

                }

            }
        }
    }

	void Runtime::start()
	{
		shouldRun = true;

		///Start Thread
		runtime = new std::thread(run, this);
	}

	void Runtime::end()
    {
        shouldRun = false;
        //runtime->join();
    }
    uint16_t Runtime::getAmountOfGroups()
    {
        return (uint16_t) groups.size();
    }

    uint64_t Runtime::getCyclesSinceClear()
    {
        return cyclesSinceLastClear;
    }

    uint32_t Runtime::getCyclesSinceManage()
    {
        return cyclesSinceLastManage;
    }
    
    float Runtime::getLastManagesAverageDelta()
    {
        return lastManagesAverageDelta;
    }

    float Runtime::getLastDelta()
    {
        return miliLastDelta;
    }

    uint32_t Runtime::getLastManagesCycles()
    {
        return lastManagesCycles;
    }

    float Runtime::getTime()
    {
        return smoothTime;
    }

    void Runtime::insertGroup(RuntimeGroup *group)
    {

        if(group->runtimeId+1 > groups.capacity())
        {
            uint32_t lastCapacity = groups.capacity();
            
            groups.resize(group->runtimeId+1);
            
            //Set all undefined groups to nullptr
            for(uint32_t i = lastCapacity; i < groups.capacity()-1; i++)
            {
                groups[i] = nullptr;
            }
        }
        groups[group->runtimeId] = group;
    }

    //DEPRECATED @DEPRECATED
    void Runtime::insertGroup(RuntimeGroup *group, uint32_t index)
    {
        Log::critErr(LOG_TAG, "Runtime::insertGroup(RuntimeGroup*, uint32_t) is now deprecated.");
        if(index+1 > groups.size())
        {
            groups.resize(index+1);
        }
        groups[index] = group;

    }
    
    RuntimeGroup* Runtime::getGroup(uint32_t pos)
    {
        return groups.at(pos);
    }

	std::string Runtime::getName()
	{
		return name;
	}

	std::thread::id Runtime::getThreadID()
	{
		return runtime->get_id();
	}

	void Runtime::cycle()
    {
        if(!staticQueue.empty())
        {
            for (int i = 0; i < staticQueue.size(); ++i)
            {
                staticQueue.front()();
                staticQueue.pop();
            }
        }

		if (!queue.empty())
		{
			for (int i = 0; i < queue.size(); ++i)
			{
				queue.front().first(queue.front().second);
				queue.pop();
			}
		}

        for (int i = 0; i < groups.size(); ++i)
        {
            if(groups[i]==nullptr)
                continue;
            groups[i]->cycle();
        }
    }

    void Runtime::enqueFunctionStatic(void (* f)())
    {
        staticQueue.push(f);
    }

	void Runtime::enqueFunction(std::pair<void(*)(void*), void*> f)
	{
		queue.push(f);
	}
}
