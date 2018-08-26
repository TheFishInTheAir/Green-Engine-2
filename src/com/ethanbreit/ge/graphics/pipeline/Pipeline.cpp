#include <ge/graphics/pipeline/Pipeline.h>
#include <ge/graphics/GraphicsCore.h>
#include <engine/global_settings.pre>
#include <ge/console/Log.h>
namespace ge
{
    // PipelineRuntimeRouter NOTE: I should prob put this in a different file 
    PipelineRuntimeRouter* PipelineRuntimeRouter::ctx;

    void PipelineRuntimeRouter::init()
    {
        ctx = new PipelineRuntimeRouter();
        ctx->runtimeId = PIPELINE_ROUTER_RG;
        RuntimeManager::getRuntime(RUNTIME_MAIN)->insertGroup(ctx);
    }

    void PipelineRuntimeRouter::cycle()
    {
        Pipeline* p = GraphicsCore::ctx->currentPipeline;
        if(p == nullptr)
            return;//when would this ever happen though....
        p->draw();
    }

    void PipelineRuntimeRouter::signal()
    {
        for(auto item : items)
            item.handler->func(item.ref,runtimeId);
    }

    // Pipeline
    PipelineStage* Pipeline::getCurrentStage()
    {
        return *currentStage;
    }

    PipelineState::type Pipeline::getState()
    {
        return currentState;
    }

    void Pipeline::draw()
    {
        static bool first = true; //NOTE: shouldn't be here
        if(first)
        {
            first = false;
            ge::GraphicsCore::ctx->window->makeCurrentThread(ge::RuntimeManager::getRuntime(RUNTIME_MAIN));
        }

        currentState = PipelineState::Render;
        currentStage = stages.begin();
        while(currentStage != stages.end())
        {
            //Draw stage
            if((*currentStage)->framebuffer==nullptr)//default fbo (0)
            {

                GraphicsCore::ctx->window->bindFrameBuffer();
                GraphicsCore::ctx->window->clear();
            }
            else
            {
                //Log::dbg("SUCCESS BABY");
                (*currentStage)->framebuffer->bind();
                (*currentStage)->framebuffer->clear();
            }
            
            PipelineRuntimeRouter::ctx->signal(); //signal all listiners to update.

            GraphicsCore::ctx->window->bindFrameBuffer();
            currentStage++;
        }
        currentState = PipelineState::PostRender;

        PipelineRuntimeRouter::ctx->signal(); //post render signal (usefull for cleanup...)

        currentState = PipelineState::NotRendering;
        
        GraphicsCore::ctx->window->swap();
    }

}