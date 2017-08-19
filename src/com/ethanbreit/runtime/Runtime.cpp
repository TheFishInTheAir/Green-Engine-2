//
// Created by Ethan Breit on 2017-08-12.
//

#include <string>
#include <runtime/Runtime.h>
#include <console/ConsoleIO.h>

namespace ge
{

    Runtime*     Runtime::passedContext;
    std::mutex   Runtime::passedContextSemaphore;

    typedef std::chrono::nanoseconds    nanoseconds;
    typedef std::chrono::seconds        seconds;
    Runtime::Runtime(std::string name, uint32_t cyclesPerSecond)
    {

        static bool shouldInitialize = true;

        if(shouldInitialize)
        {
            shouldInitialize=false;
            passedContextSemaphore.unlock();
        }
        for ( auto g : groups )
            g = nullptr;
        this->name = name;
        this->cyclesPerSecond = cyclesPerSecond;

        shouldRun = true;
        ///Start Instance Pass

        passedContextSemaphore.lock();
        passedContext = this;

        ///Start Thread
        runtime = new std::thread(run);
    }
    void Runtime::run()
    {

        ///Acquire Instance

        Runtime *context = passedContext;
        passedContextSemaphore.unlock();

        ///Initialise Cycle Timing Data
        nanoseconds delta(nanoseconds(seconds(1)).count()/context->cyclesPerSecond); ///Calculate Delta

        std::chrono::steady_clock::time_point nextCycle(std::chrono::steady_clock::now()); ///Time to next cycle
        nextCycle += (delta);

        ///Initialise Cycle Manager Timing Data
        nanoseconds managerDelta(nanoseconds(std::chrono::seconds(1)));
        std::chrono::steady_clock::time_point nextManage(std::chrono::steady_clock::now());
        nextManage += managerDelta;


        std::chrono::steady_clock::time_point currentTime;


        ConsoleIO::Print("Starting runtime \""+context->name+"\" at "+std::to_string(context->cyclesPerSecond)+" cycles per second.\n", MessageType::Message);

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


                nextManage =  currentTime;
                nextManage += managerDelta;

                ///Correct Delta

                if(!context->cyclesSinceLastManage*context->managesSinceLastClear==context->cyclesSinceLastClear)
                {
                    uint64_t incorrect = context->cyclesSinceLastManage*context->managesSinceLastClear;
                    uint64_t correct   = context->cyclesSinceLastClear;

                    uint32_t difference;

                    difference = (uint32_t) (correct - incorrect); /// get the discrepancy

                    delta = nanoseconds(nanoseconds(seconds(1)).count()/(context->cyclesPerSecond+difference)); ///recalculate Delta (to account for the discrepancy)

                }
                else
                {
                    delta = nanoseconds(nanoseconds(seconds(1)).count()/context->cyclesPerSecond); ///recalculate Delta
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

    void Runtime::cycle()
    {
        if(!queue.empty())
        {
            for (int i = 0; i < queue.size(); ++i)
            {
                queue.front()();
                queue.pop();
            }
        }

        for (int i = 0; i < groups.size(); ++i)
        {
            groups[i]->cycle();
        }
    }

    void Runtime::enqueFunction(void (* f)())
    {
        queue.push(f);
    }

}