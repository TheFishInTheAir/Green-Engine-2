#pragma once
#include <unordered_map>
#include <string>
#include <cstdlib>
#include "ReadableMemType.h"

namespace ge
{

	namespace GlobalMemory
    {
        struct MemItem
        {
            MemItem(void* mem, ReadableMemType::type memType)
            {
                data = mem;
                memoryType = memType;
            }

            template <typename Type> Type* getRawData()
            {

                //TODO: super unsafe, come up with a better way to implement this.
                Type* t = reinterpret_cast<Type*> (data);
                return t;
            }

            template <typename Type> Type getData()
            {

                //TODO: super unsafe, come up with a better way to implement this.
                Type t = *(reinterpret_cast<Type*> (data));
                return t;
            }

            void* data;
            ReadableMemType::type memoryType;
        };
        void insert(std::string, MemItem);

        void remove(std::string);

        MemItem get(std::string);

        void deInit();

        void init();

    }
}