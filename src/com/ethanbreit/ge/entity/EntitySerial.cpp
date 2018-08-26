#include <ge/entity/EntitySerial.h>
#include <ge/entity/Entity.h>
#include <ge/entity/EntityTag.h>
#include <ge/entity/EntityManager.h>
#include <ge/entity/component/Component.h>
#include <ge/entity/component/ComponentConstructorRegistry.h>
#include <ge/console/Log.h>


namespace ge
{
    namespace EntitySerial
    {
        const std::string LOG_TAG = "EntitySerial";

        nlohmann::json serializeEntity(Entity* ent) //TODO: add Entity Tags
        {
            nlohmann::json jEnt;

            //Early escapes
            if(ent==nullptr) //in the case of an  unoptimized ent list iteration
                Log::critErr(LOG_TAG, "Attempt to serialize null entity."); //return empty

            if (EntityManager::tagExists("no_export"))
            {
                bool shouldSkip = false;

                for (auto i : ent->tags)
                    if (i->name == EntityManager::getTag("no_export")->name)
                    {
                        shouldSkip = true;
                        break;
                    }

                if (shouldSkip)
                {   
                    Log::wrn(LOG_TAG, "Attempt to serialize 'no_export' Entity, returning empty Json.");
                    return jEnt;
                }
            }

            //Serialization
            std::list<nlohmann::json> jCmps;
            for (auto cm : ent->components)
            {
                nlohmann::json cmp;

                std::list<nlohmann::json> jVars;
                for (auto v : cm.second->publicVars)
                    jVars.push_back(serializePublicVar(cm.second, v.first));
                cmp["type"] = cm.first;
                cmp["vars"] = jVars;
                jCmps.push_back(cmp);
            }
            std::list<nlohmann::json> jTags;
            for (auto tag : ent->tags)
                jTags.push_back(nlohmann::json(tag->name));


            jEnt["name"] = ent->name;
            jEnt["components"] = jCmps;
            jEnt["tags"] = jTags;
            return jEnt;
        }

        Entity* deserializeEntity(nlohmann::json jEnt)
        {
            Entity* ent = new Entity();
            ent->name = jEnt["name"];
            for (nlohmann::json jCmp : jEnt["components"])
            {
                Component *cmp = ComponentConstructorRegistry::newComponent(jCmp["type"], ent);
                ent->insertComponent(cmp);
                for(nlohmann::json jVar : jCmp["vars"])
                    deserializePublicVar(cmp, jVar);
            }

            for(nlohmann::json jTags : jEnt["tags"])
                ent->tags.push_back(EntityManager::getOrCreateTag(jTags));
            
            return ent;
        }

        nlohmann::json serializePublicVar(Component *src, std::string vTag)
        {
            nlohmann::json pvar;
            auto srcVar = src->publicVars[vTag];

            pvar["name"] = vTag;
            pvar["value"] = ge::DataType::serialize(srcVar.first, srcVar.second);

            return pvar;
        }

        void deserializePublicVar(Component* cmp, nlohmann::json jVar)
        {
            if(!jVar.count("name"))
                return Log::err(LOG_TAG, "Malformed Json Public Var SKIPPING");

            if(!cmp->publicVars.count(jVar["name"]))
                return Log::err(LOG_TAG, "Component '"+cmp->getTypeName()+"' doesn't contain public value '"+jVar["name"].get<std::string>()+"' SKIPPING");
            
            auto pv = cmp->publicVars[jVar["name"]];
            DataType::deserialize(jVar["value"], pv.first, pv.second);
        }

        void copyPublicVar(Component* dest, Component* src, std::string v)
        {
            if(!dest->publicVars.count(v))
                return Log::err(LOG_TAG, "Component '"+dest->getTypeName()+"' doesn't contain public value '"+ v +"' SKIPPING");
            if(!src->publicVars.count(v))
                return Log::err(LOG_TAG, "Component '"+src->getTypeName()+"' doesn't contain public value '"+ v +"' SKIPPING");
            DataType::copy(dest->publicVars[v].first, src->publicVars[v].second, dest->publicVars[v].second);
        }
    }
}