#include <memory/GlobalMemory.h>
#include <mutex>

namespace ge
{
    namespace GlobalMemory
    {
        std::unordered_map<std::string, MemItem> _mem;

        std::mutex _accessSemaphore;

        void insert(std::string key, MemItem o)
        {

            _accessSemaphore.lock();

            _mem.insert({key, o});

            _accessSemaphore.unlock();

        }

        void remove(std::string key)
        {
            _accessSemaphore.lock();

            _mem.erase(key);

            _accessSemaphore.unlock();

        }

        MemItem get(std::string key)
        {

            _accessSemaphore.lock();

            MemItem item = _mem.find(key)->second;
            _accessSemaphore.unlock();
            return item;

        }

        void deInit()
        {
            _mem.clear();
        }

        void init()
        {
            _accessSemaphore.unlock();
        }
    }
}
