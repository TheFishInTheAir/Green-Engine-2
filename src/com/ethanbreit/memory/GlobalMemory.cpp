#include <memory/GlobalMemory.h>
#include <mutex>

namespace ge
{
    namespace GlobalMemory
    {
        std::unordered_map<std::string, MemItem> _mem;

        std::mutex _accessSemaphore;

		std::vector<std::string> getAllEntries()
	    {
			_accessSemaphore.lock();

			std::vector<std::string> out;
			out.resize(_mem.size());

			int i = 0;
			for(auto m : _mem)
			{
				
				out[i] = m.first;

				i++;
			}

			_accessSemaphore.unlock();
			return out;
	    }

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

	    bool exists(std::string key)
	    {
			_accessSemaphore.lock();
			bool state = _mem.count(key) != 0;
			_accessSemaphore.unlock();
			return state;
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
    }
}
