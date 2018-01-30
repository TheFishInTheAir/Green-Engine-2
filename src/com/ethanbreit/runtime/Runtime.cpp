//
// Created by Ethan Breit on 2017-08-12.
//

#include <string>
#include <runtime/Runtime.h>
#include <console/ConsoleIO.h>
#include "memory/GlobalMemory.h"

namespace ge
{



    typedef std::chrono::nanoseconds    nanoseconds;
    typedef std::chrono::seconds        seconds;
    Runtime::Runtime(std::string name, uint32_t cyclesPerSecond)
    {

        for ( auto g : groups )
            g = nullptr;
        this->name = name;
        this->cyclesPerSecond = cyclesPerSecond;

        shouldRun = true;
        ///Start Instance Pass

        ///Start Thread

		runtime = new std::thread(run, this);	


		GlobalMemory::insert("ge_"+name+"_context_runtime", { this,ReadableMemType::OTHER });
    }

	Runtime::Runtime(std::string name)
	{
		for (auto g : groups)
			g = nullptr;
		this->name = name;
		this->cyclesPerSecond = cyclesPerSecond;

		shouldRun = true;
		///Start Instance Pass

		///Start Thread

		runtime = new std::thread(runUnlocked, this);
		GlobalMemory::insert("ge_" + name + "_context_runtime", { this,ReadableMemType::OTHER });

	}

	void Runtime::run(Runtime* context)
    {

        ///Acquire Instance


        ///Initialise Cycle Timing Data
        nanoseconds delta(nanoseconds(seconds(1)).count()/context->cyclesPerSecond); ///Calculate Delta

        std::chrono::steady_clock::time_point nextCycle(std::chrono::steady_clock::now()); ///Time to next cycle
        nextCycle += (delta);

        ///Initialise Cycle Manager Timing Data
        nanoseconds managerDelta(nanoseconds(std::chrono::seconds(1)));
        std::chrono::steady_clock::time_point nextManage(std::chrono::steady_clock::now());
        nextManage += managerDelta;


        std::chrono::steady_clock::time_point currentTime;


        ConsoleIO::print("Starting runtime \""+context->name+"\" at "+std::to_string(context->cyclesPerSecond)+" cycles per second.\n", MessageType::Message);

        while (context->shouldRun)
        {

            currentTime = std::chrono::steady_clock::now();



            if(currentTime>=nextCycle)
            {
                nextCycle =  currentTime;
                nextCycle += delta;
                context->cyclesSinceLastClear++;
                context->cyclesSinceLastManage++;

                context->cycle(); /// Cycle Groups;
            }

            if(currentTime>=nextManage)
            {

                context->managesSinceLastClear++;

				//TESTING
				ConsoleIO::print(context->name+": "+std::to_string(context->cyclesSinceLastManage)+"\n");
				//TESTING
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

	void Runtime::runUnlocked(Runtime* context)
	{
		ConsoleIO::print("Starting runtime \"" + context->name + "\" at unlocked cycles per second.\n", MessageType::Message);


		std::chrono::steady_clock::time_point lastManage = std::chrono::steady_clock::now();

		std::chrono::steady_clock::time_point currentTime;
		context->cyclesSinceLastManage = 0;
		while (context->shouldRun)
		{

			currentTime = std::chrono::steady_clock::now();

			context->cycle();
			context->cyclesSinceLastManage++;
			if (currentTime >= lastManage + std::chrono::seconds(1))
			{
				lastManage = std::chrono::steady_clock::now();
				ConsoleIO::print(context->name + ": " + std::to_string(context->cyclesSinceLastManage) + "\n");
				context->cyclesSinceLastManage = 0;
			}
		}
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

    void Runtime::insertGroup(RuntimeGroup *group)
    {
        groups.push_back(group);
    }

    void Runtime::insertGroup(RuntimeGroup *group, uint32_t index)
    {
        if(index+1 > groups.size())
        {
            groups.resize(index+1);
        }
        groups[index] = group;

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
