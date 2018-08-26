#include <ge/debug/DebugGUI.h>

#define GE_NK_GLFW_3

//NOTE: this uses OpenGL and I am trying to make this API agnostic so, TODO: make good
#include <ge/console/Log.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>


#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION

//#define NK_GLFW_GL3_IMPLEMENTATION

#include <nuklear.h> //TODO: use GELly when the port is done
#ifdef GE_NK_GLFW_4
#define NK_GLFW_GL4_IMPLEMENTATION
#include <nuklear_glfw_gl4.h>
#elif defined GE_NK_GLFW_3
#define NK_GLFW_GL3_IMPLEMENTATION
#include <nuklear_glfw_gl3.h>
#endif

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

//GL
#include <ge/graphics/abs/OpenGL/types/GLWindow.h>
#include <ge/graphics/abs/OpenGL/types/GLTexture.h>

#include <ge/graphics/GraphicsCore.h>
#include <ge/input/KeyboardHandler.h>
#include <ge/input/MouseHandler.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntitySerial.h>
#include <ge/entity/component/Component.h>
#include <ge/engine/scene/Scene.h>
#include <ge/engine/scene/SceneSerializer.h>
#include <ge/util/ResourceUtil.h>
#include <ge/runtime/Runtime.h>
#include <ge/runtime/RuntimeManager.h>
#include <ge/entity/component/ComponentBatch.h>
#include <ge/entity/component/ComponentConstructorRegistry.h>

namespace ge
{
    namespace Debug
    {
        DebugGUI* DebugGUI::ctx;

        const std::string LOG_TAG = "DebugGUI";

        void DebugGUI::enable()
        {
            if(ctx==nullptr)
            {
                ctx = new DebugGUI();
                ctx->init();
            }
            ctx->isEnabled = true;
        }

        void DebugGUI::disable()
        {
            if(ctx==nullptr)
                Log::critErr(LOG_TAG, "Attempt to disable uninitialised DebugGUI. (enable to init)");

            ctx->isEnabled = false;
        }

        void DebugGUI::draw() //split into sub funcs
        {

            if(!isEnabled)
                return;

            if(GraphicsCore::ctx->currentPipeline->getState()!=PipelineState::PostRender)
                return;

            {
                static bool justPresesd = false;
                if(ge::KeyboardHandler::keyDownSticky(ge::KeyboardKey::TILDA))
                {
                    if(!justPresesd)
                    {
                        if(shouldRender)
                        { //CLOSE
							MouseHandler::queueMouseDisableStateChange(true);
                            //glfwSetInputMode(((ge::GL::Window*) GraphicsCore::ctx->window)->_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                            MouseHandler::disabled = false;
                        }
                        else
                        { //OPEN NOTE: only works on osx
							MouseHandler::queueMouseDisableStateChange(false);
							MouseHandler::disabled = true;

                        }
                        shouldRender = !shouldRender;
                    }
                    justPresesd = true;
                }
                else
                {
                    justPresesd = false;
                }
                
            }
            
            nk_glfw3_new_frame();

            if(shouldDrawInfoOverlay)
                drawInfoOverlay();


            if(shouldRender)
            { 
                if (nk_begin(nctx, "DebugGUI", nk_rect(50, 50, 230, 250),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
                {
                    nk_layout_row_dynamic(nctx, 30, 1);
                    if(nk_button_label(nctx, "Toggle Entity Tree"))
                        shouldDrawEntTree = !shouldDrawEntTree;
                    if(nk_button_label(nctx, "Toggle Scene Probe"))
                        shouldDrawCurrentRes = !shouldDrawCurrentRes;
                    if(nk_button_label(nctx, "Toggle Info Overlay"))
                        shouldDrawInfoOverlay = !shouldDrawInfoOverlay;
                    if(nk_button_label(nctx, "Toggle FrameBuffer View"))
                        shouldDrawFrameBuffers = !shouldDrawFrameBuffers;
                    //if(nk_button_label(nctx, "Toggle Pipeline View"))
                      //  shouldDrawPipeline = !shouldDrawPipeline;
                        
                }
                nk_end(nctx);

                if(shouldDrawEntTree)
                    drawEntTree();
                if(shouldDrawCurrentRes)
                    drawSceneProbe();
                if(selectedEnt!=nullptr)
                    drawEntProbe();
                if(shouldDrawFrameBuffers)
                    drawFrameBuffers();
            }
            //Log::dbg("TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1TEST1");
            //nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
#ifdef GE_NK_GLFW_4
			nk_glfw3_render(NK_ANTI_ALIASING_ON);
#elif defined GE_NK_GLFW_3
            nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
#endif			//Log::dbg("TEST2");

            glEnable(GL_DEPTH_TEST); // shouldn't be here


        }
        

        void DebugGUI::drawEntTree()
        {
            
            if (nk_begin(nctx, "DebugGUI: Entity Tree", nk_rect(350, 50, 230, 250),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
                {
                    nk_layout_row_dynamic(nctx, 25, 1);
                    if(nk_button_label(nctx, "New Entity"))
                    {
                        Entity* ent = new Entity();
                        ent->name = "new_entity";

                        EntityManager::registerEntity(ent);
                    }
                    nk_layout_row_dynamic(nctx, 20, 1);

                    for(Entity* ent : *EntityManager::getAllEntities())
                    {
                        if(ent==nullptr)
                            continue;
                        /*
                        if (nk_tree_push_id(nctx, NK_TREE_TAB, ent->name.c_str(), NK_MINIMIZED, ent->id*1000)) 
                        {
                            int i = 0;

                            for(auto c : ent->components)
                            {
                                if(nk_tree_push_id(nctx, NK_TREE_TAB, c.first.c_str(), NK_MINIMIZED, (ent->id*1000)+i++))
                                {
                                    nk_layout_row_dynamic(nctx, 20, 1);
                                    nk_label(nctx, "TEST DATA", NK_TEXT_LEFT);

                                    nk_tree_pop(nctx);
                                }
                            }
                            nk_tree_pop(nctx);
                        }*/
                        nk_layout_row_dynamic(nctx, 20, 1);
                        if(nk_button_label(nctx, ent->name.c_str()))
                            selectedEnt = ent;
                    }
                }
                nk_end(nctx);
        }

        void DebugGUI::drawFrameBuffers()
        {
            //auto lastStyle = nctx->style.window.fixed_background;
            //nctx->style.window.fixed_background = nk_style_item_color(nk_rgba(0,0,0,0));
            int x, y;
            GraphicsCore::ctx->window->getSize(&x, &y);
            if (nk_begin(nctx, "DebuGUI: FrameBuffer Overlay", nk_rect(0, y*0.8, x, y*0.30f), 0))
            {
                //nk_layout_row_dynamic(nctx,20,1);
                if(Scene::currentScene->shadows.empty())
                {
                    nk_end(nctx);
                    return;
                }
                int texId = (int)((ge::GL::Texture*)(Scene::currentScene->shadows.front()->shadowMap))->id;
                glBindTexture(GL_TEXTURE_2D, texId);
                auto img = nk_image_id(texId);
                struct nk_vec2 upperCord = nk_window_get_content_region_min(nctx);
            
                //nk_stroke_line(nk_window_get_canvas(nctx), upperCord.x, upperCord.y, upperCord.x+30, upperCord.y+30, 1.0f, nk_rgba(255,255,100,255));
                nk_draw_image(nk_window_get_canvas(nctx),nk_rect(upperCord.x,upperCord.y,y*0.30f,y*0.30f),&img,nk_rgba(255,255,255,255));
            }
            nk_end(nctx);

            //nctx->style.window.fixed_background = lastStyle;
        }

        void DebugGUI::drawEntProbe()
        {
            if (nk_begin(nctx, "DebuGUI: Entity Probe", nk_rect(750, 50, 330, 550),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
            {
                nk_layout_row_dynamic(nctx, 25, 1);
                if(nk_button_label(nctx, "Delete Entity"))
                {
                    selectedEnt->destroy();
                    EntityManager::removeEntity(selectedEnt);

                    free(selectedEnt);

                    selectedEnt = nullptr;
                    nk_end(nctx);

                    return;
                }

                if(nk_button_label(nctx, "Duplicate"))
                {
                    //Log::err("unimplemented");
                    //return;

                    Entity* ent = new Entity();
                    ent->name = selectedEnt->name+"-dupe";
                    for(auto cmp : selectedEnt->components)
                    {
                        Component* nCmp = ComponentConstructorRegistry::newComponent(cmp.first, ent);
                        for(auto pv : cmp.second->publicVars)
                        {
                            EntitySerial::copyPublicVar(nCmp, cmp.second, pv.first);
                        }
                        ent->insertComponent(nCmp);

                        nCmp->insertToDefaultBatch();
                    }
                    EntityManager::registerEntity(ent);
                }

                nk_layout_row_dynamic(nctx, 25, 2);
                nk_label(nctx, std::string("Entity Name: ").c_str(), NK_TEXT_LEFT);
                {
                    char* editStr = (char*) malloc(selectedEnt->name.size()+1);
                    strcpy(editStr, selectedEnt->name.c_str());
                    nk_edit_string_zero_terminated(nctx, NK_EDIT_SIMPLE, editStr, 64, 0);
                    selectedEnt->name = std::string(editStr);
                    free (editStr);
                }
                //nk_label(nctx, std::string("Entity Name: "+selectedEnt->name).c_str(), NK_TEXT_LEFT);
                nk_layout_row_dynamic(nctx, 20, 1);
                nk_label(nctx, std::string("Entity Id: "+std::to_string(selectedEnt->id)).c_str(), NK_TEXT_LEFT);



                { //COMPONENTS
                    nk_layout_row_dynamic(nctx, 20, 1);

                    int i = 0;
                    for(auto c : selectedEnt->components)
                    {
                        if (nk_tree_push_id(nctx, NK_TREE_TAB, c.first.c_str(), NK_MINIMIZED, i++)) 
                        {
                            if(nk_button_label(nctx, "Destroy"))
                            {
                                selectedEnt->removeComponent(c.first);  //NOTE: concurrent modification?

                                c.second->getBatch()->hardRemove(c.second->batchId);

                                c.second->destroy();

                                free(c.second);
                                
                                nk_tree_pop(nctx);
                                nk_end(nctx);


                                return; //NOTE: continue not working...
                            }


                            for(std::string vn : c.second->publicVarsDisplayOrder)
                            {
                                auto v = c.second->publicVars.find(vn);
                                dtfAuto(v->first, v->second.first, v->second.second);
                            }
                            nk_tree_pop(nctx);
                        }
                    }
                }
                nk_layout_row_dynamic(nctx, 25, 1);

                static bool addComponentMenuOpen = false;
                if(nk_button_label(nctx, "Add Component") || addComponentMenuOpen)
                {
                    
                    addComponentMenuOpen = true;
                    if(nk_popup_begin(nctx, NK_POPUP_STATIC, "DebugGUI: Add Component", 0, nk_rect(10, 10, 200, 300)))
                    {
                        nk_layout_row_dynamic(nctx, 20, 1);
                        if(nk_button_label(nctx, "cancel"))
                        {
                            addComponentMenuOpen = false;
                            nk_popup_close(nctx);
                            nk_popup_end(nctx);
                            nk_end(nctx);
                            return;
                        }
                        nk_layout_row_dynamic(nctx, 20, 1);
                        nk_layout_row_dynamic(nctx, 20, 1);
                        for(auto s : ComponentConstructorRegistry::getAvailable())
                        {
                            if(selectedEnt->components.count(s))
                                continue;
                            if(nk_button_label(nctx, s.c_str()))
                            {
                                if(selectedEnt->components.count(s))
                                    break;

                                Component* cmp = ComponentConstructorRegistry::newComponent(s, selectedEnt);

                                cmp->insertToDefaultBatch();
                                selectedEnt->insertComponent(cmp);
                                addComponentMenuOpen=false;
                                nk_popup_close(nctx);
                            } 
                        }
                        nk_popup_end(nctx);
                    }
                }
            }
            nk_end(nctx);
        }

        void DebugGUI::drawInfoOverlay()
        {
            Runtime* r = RuntimeManager::getRuntime(RUNTIME_MAIN);

            auto lastStyle = nctx->style.window.fixed_background;
            nctx->style.window.fixed_background = nk_style_item_color(nk_rgba(0,0,0,0));

            if (nk_begin(nctx, "DebuGUI: Info Overview", nk_rect(0, 0, 200, 160), 0))
            {
                nk_layout_row_dynamic(nctx,20,1);
                //Runtime's target FPS
                nk_label(nctx, (std::string("Target FPS: ")+(r->cyclesPerSecond == 0 ? "Unlocked" : "t"+std::to_string(r->cyclesPerSecond))).c_str(), NK_TEXT_LEFT);
                
                //LastManages FPS
                nk_label(nctx, (std::string("Last FPS: ")+std::to_string(r->getLastManagesCycles())).c_str(), NK_TEXT_LEFT);

                //LastManages Delta
                nk_label(nctx, (std::string("Avg Delta: ")+std::to_string(r->getLastManagesAverageDelta())).c_str(), NK_TEXT_LEFT);
            }
            nk_end(nctx);

            nctx->style.window.fixed_background = lastStyle;
        }

        void DebugGUI::drawSceneProbe()
        {
            
            if (nk_begin(nctx, "DebuGUI: Scene", nk_rect(550, 50, 230, 240),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
                NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
            {
                nk_layout_row_dynamic(nctx, 15, 1);
                nk_label(nctx, (std::string()+"Current Scene: "+Scene::currentScene->url).c_str(), NK_TEXT_LEFT);
                
                nk_layout_row_dynamic(nctx, 25, 2);
                if(nk_button_label(nctx, "Save Scene"))
                {
                    //Call Scene Serializer
                    if(probeSaveLoc=="")
                        Log::msg(SceneSerializer::serializeCurrentSceneJson().dump(2));
                    else
                        ResourceUtil::writeRawStrResource(probeSaveLoc, SceneSerializer::serializeCurrentSceneJson().dump(2));
                }
                {//TextEdit
                    char* editStr = (char*) malloc(probeSaveLoc.size()+2);
                    strcpy(editStr, probeSaveLoc.c_str());
                    nk_edit_string_zero_terminated(nctx, NK_EDIT_SIMPLE, editStr, 64, 0);
                    probeSaveLoc = std::string(editStr);
                    free (editStr);
                }
                nk_layout_row_dynamic(nctx, 25, 2);
                if(nk_button_label(nctx, "Load Scene"))
                {
                    //Call Scene Loader

                }
                {//TextEdit
                    char* editStr = (char*) malloc(probeLoadLoc.size()+1);
                    strcpy(editStr, probeLoadLoc.c_str());
                    nk_edit_string_zero_terminated(nctx, NK_EDIT_SIMPLE, editStr, 64, 0);
                    probeLoadLoc = std::string(editStr);
                    free (editStr);
                }

                if(nk_tree_push(nctx, NK_TREE_TAB, "Loaded Resources", NK_MINIMIZED))
                {
                    if(nk_tree_push(nctx, NK_TREE_TAB, "Meshes", NK_MINIMIZED))
                    {
                        for(auto m : Scene::currentScene->meshes)
                        {
                            nk_label(nctx, m.first.c_str(), NK_TEXT_LEFT);
                        }
                        nk_tree_pop(nctx);
                    }
                    if(nk_tree_push(nctx, NK_TREE_TAB, "Textures", NK_MINIMIZED))
                    {
                        for(auto t : Scene::currentScene->textures)
                        {
                            nk_label(nctx, t.first.c_str(), NK_TEXT_LEFT);
                        }
                        nk_tree_pop(nctx);
                    }
                    if(nk_tree_push(nctx, NK_TREE_TAB, "Cubemaps", NK_MINIMIZED))
                    {
                        for(auto cm : Scene::currentScene->cubemaps)
                        {
                            nk_label(nctx, cm.first.c_str(), NK_TEXT_LEFT);
                        }
                        nk_tree_pop(nctx);
                    }
                    if(nk_tree_push(nctx, NK_TREE_TAB, "ShaderGroups", NK_MINIMIZED))
                    {
                        for(auto sg : Scene::currentScene->shaderGroups)
                        {
                            nk_label(nctx, sg.first.c_str(), NK_TEXT_LEFT);
                        }
                        nk_tree_pop(nctx);
                    }   
                    if(nk_tree_push(nctx, NK_TREE_TAB, "Shaders", NK_MINIMIZED))
                    {
                        for(auto s : Scene::currentScene->shaders)
                        {
                            nk_label(nctx, s.first.c_str(), NK_TEXT_LEFT);
                        }
                        nk_tree_pop(nctx);
                    }
                    if(nk_tree_push(nctx, NK_TREE_TAB, "Materials", NK_MINIMIZED))
                    {
                        for(auto s : Scene::currentScene->materials)
                        {
                            nk_label(nctx, s.first.c_str(), NK_TEXT_LEFT);
                        }
                        nk_tree_pop(nctx);
                    }
                    nk_tree_pop(nctx);
                }
            }
            nk_end(nctx);
        }
        void DebugGUI::init()
        {
            ge_REGISTER_RUNTIME_HANDLER

            ge::GL::Window* w = (ge::GL::Window*) GraphicsCore::ctx->window;

#ifdef GE_NK_GLFW_4
            nctx = nk_glfw3_init(w->_window, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER); //last two are arbitrary defaults
#elif defined GE_NK_GLFW_3
            nctx = nk_glfw3_init(w->_window, NK_GLFW3_INSTALL_CALLBACKS); //last two are arbitrary defaults
#endif
            {struct nk_font_atlas *atlas;
            nk_glfw3_font_stash_begin(&atlas);
            /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
            /*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 14, 0);*/
            /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
            /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
            /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
            /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
            nk_glfw3_font_stash_end();
            /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
            /*nk_style_set_font(ctx, &droid->handle);*/}


            RuntimeManager::getRuntime(RUNTIME_MAIN)->getGroup(PIPELINE_ROUTER_RG)->
            ge_RUNTIME_GROUP_INSERT_HEAP(this);
        }



        /**
         * 
         *  Data Type Formatting
         * 
         */

        void DebugGUI::dtfAuto(std::string name, DataType::type t, void* v)
        {
            switch(t)
            {
                case DataType::BOOL:
                    dtfBool(name, (bool*)v);
                    break;
                case DataType::FLOAT:
                    dtfFloat(name, (float*)v);
                    break;
                case DataType::INT:
                    dtfInt(name, (int*)v);
                    break;
                case DataType::FVEC3:
                    dtfFVEC3(name, (glm::vec3*)v);
                    break;
                case DataType::QUAT:
                    dtfQUAT(name, (glm::quat*)v);
                    break;
                case DataType::COLOUR3:
                    dtfCOL3(name, (glm::vec3*)v);
                    break;
                case DataType::MATERIAL:
                    dtfMat(name, (std::string*) v);
                    break;
                case DataType::MESH_DATA:
                    dtfMD(name, (std::string*) v);
                    break;
            }
        }

        void DebugGUI::dtfBool(std::string name, bool *v)
        {
            nk_layout_row_begin(nctx, NK_DYNAMIC, 30, 2);
            nk_layout_row_push(nctx, 0.66666f);

            nk_label(nctx, name.c_str(), NK_TEXT_LEFT);

            nk_layout_row_push(nctx, 0.33333f);

            if(nk_button_label(nctx, (*v ? "True" : "False")))
                *v = !*v;

            nk_layout_row_end(nctx);
        }

        void DebugGUI::dtfMat(std::string name, std::string* text)
        {
            nk_layout_row_begin(nctx, NK_DYNAMIC, 30, 2);
            nk_layout_row_push(nctx, 0.66666f);


            nk_label(nctx, name.c_str(), NK_TEXT_LEFT);

            nk_layout_row_push(nctx, 0.33333f);

            static std::string* currentSelected = nullptr;
            if(nk_button_label(nctx, text->c_str()))
                currentSelected = text;

            if(currentSelected != nullptr)
                if(nk_popup_begin(nctx, NK_POPUP_STATIC, "DebugGUI: Material Selector", 0, nk_rect(10, 10, 200, 300)))
                {
                    nk_layout_row_dynamic(nctx, 20, 1);
                    for(auto s : Scene::currentScene->materials)
                    {
                        if(nk_button_label(nctx, s.second.name.c_str()))
                        {
                            *currentSelected = s.second.name;
                            currentSelected = nullptr;
                            nk_popup_close(nctx);
                        }
                    }
                    nk_popup_end(nctx);
                }

            
            nk_layout_row_end(nctx);
        }


        void DebugGUI::dtfMD(std::string name, std::string* text)
        {
            nk_layout_row_begin(nctx, NK_DYNAMIC, 30, 2);
            nk_layout_row_push(nctx, 0.66666f);


            nk_label(nctx, name.c_str(), NK_TEXT_LEFT);

            nk_layout_row_push(nctx, 0.33333f);

            static std::string* currentSelected = nullptr;
            if(nk_button_label(nctx, text->c_str()))
                currentSelected = text;

            if(currentSelected != nullptr)
                if(nk_popup_begin(nctx, NK_POPUP_STATIC, "DebugGUI: Mesh Data Selector", 0, nk_rect(10, 10, 200, 300)))
                {
                    nk_layout_row_dynamic(nctx, 15, 1);
                    for(auto s : Scene::currentScene->meshes)
                    {
                        if(nk_button_label(nctx, s.first.c_str()))
                        {
                            *currentSelected = s.first;
                            currentSelected = nullptr;
                            nk_popup_close(nctx);
                        }
                    }
                    nk_popup_end(nctx);
                }
                
            
            nk_layout_row_end(nctx);
        }

        void DebugGUI::dtfFloat(std::string name, float *v)
        {
            nk_layout_row_begin(nctx, NK_DYNAMIC, 30, 2);
            nk_layout_row_push(nctx, 0.66666f);

            nk_label(nctx, name.c_str(), NK_TEXT_LEFT);

            nk_layout_row_push(nctx, 0.33333f);

            nk_property_float(nctx, "#", -1000000, v, 1000000, 0.10f, 0.1f);

            nk_layout_row_end(nctx);
        }
        void DebugGUI::dtfInt(std::string name, int *v)
        {
            nk_layout_row_begin(nctx, NK_DYNAMIC, 30, 2);
            nk_layout_row_push(nctx, 0.66666f);

            nk_label(nctx, name.c_str(), NK_TEXT_LEFT);

            nk_layout_row_push(nctx, 0.33333f);

            nk_property_int(nctx, "#", -1000, v, 1000, 1, 0.1f);
            

            nk_layout_row_end(nctx);
        }
        void DebugGUI::dtfQUAT(std::string name, glm::quat *v)
        {
            nk_layout_row_begin(nctx, NK_DYNAMIC, 30, 2);
            nk_layout_row_push(nctx, 0.2);

            nk_label(nctx, name.c_str(), NK_TEXT_LEFT);

            nk_layout_row_push(nctx, 0.8);
            glm::vec3 euler = glm::degrees(glm::eulerAngles(*v));
            float f = glm::radians(0.0f);
            

            ///Too complicated for a normal user
            /*if(nk_group_begin(nctx, "float_input_group", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_layout_row_dynamic(nctx, 20,4);
                nk_property_float(nctx, "#", -1000000, &v->w, 1000000, 0.10f, 0.1f);
                nk_property_float(nctx, "#", -1000000, &v->x, 1000000, 0.10f, 0.1f);
                nk_property_float(nctx, "#", -1000000, &v->y, 1000000, 0.10f, 0.1f);
                nk_property_float(nctx, "#", -1000000, &v->z, 1000000, 0.10f, 0.1f);

                nk_group_end(nctx);
            }*/

            if(nk_group_begin(nctx, "float_input_group", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_layout_row_dynamic(nctx, 20,3);
                nk_property_float(nctx, "#", -1000000, &(euler.x), 1000000, 0.50f, 0.5f);
                nk_property_float(nctx, "#", -1000000, &(euler.y), 1000000, 0.50f, 0.5f);
                nk_property_float(nctx, "#", -1000000, &(euler.z), 1000000, 0.50f, 0.5f);

                nk_group_end(nctx);
            }

            *v = glm::quat(glm::radians(euler));
            nk_layout_row_end(nctx);

        }
        void DebugGUI::dtfFVEC3(std::string name, glm::vec3 *v)
        {
            nk_layout_row_begin(nctx, NK_DYNAMIC, 30, 2);
            nk_layout_row_push(nctx, 0.25);

            nk_label(nctx, name.c_str(), NK_TEXT_LEFT);

            nk_layout_row_push(nctx, 0.75);

            if(nk_group_begin(nctx, "float_input_group", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_layout_row_dynamic(nctx, 20,3);
                nk_property_float(nctx, "#", -1000000, &v->x, 1000000, 0.10f, 0.1f);
                nk_property_float(nctx, "#", -1000000, &v->y, 1000000, 0.10f, 0.1f);
                nk_property_float(nctx, "#", -1000000, &v->z, 1000000, 0.10f, 0.1f);

                nk_group_end(nctx);
            }

            nk_layout_row_end(nctx);

        }


        void DebugGUI::dtfCOL3(std::string name, glm::vec3 *v)
        {
            nk_layout_row_begin(nctx, NK_DYNAMIC, 30, 2);
            nk_layout_row_push(nctx, 0.25);

            nk_label(nctx, name.c_str(), NK_TEXT_LEFT);

            nk_layout_row_push(nctx, 0.75);
            nk_colorf tempC;
            tempC.r = v->x;
            tempC.g = v->y;
            tempC.b = v->z;

            tempC = nk_color_picker(nctx, tempC, NK_RGB);//TODO: BULLSHIT

            v->x = tempC.r;
            v->y = tempC.g;
            v->z = tempC.b;
            nk_layout_row_end(nctx);

        }

    }
}