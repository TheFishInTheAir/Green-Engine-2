#include <Memory/GlobalMemory.h>
#include <Debug/PrintUtil.h>
#include "../../../../../Include/com/ethanbreit/GE/Memory/GlobalMemory.h"

std::unordered_map<std::string, MemItem *>* GlobalMemory::_mem;
Semaphore GlobalMemory::_accessSemaphore;

void GlobalMemory::insert(std::string key, MemItem* o)
{

    _accessSemaphore.acquire();

    if(_mem != NULL)
        _mem->insert({ key, o });

    _accessSemaphore.release();

}

void GlobalMemory::remove(std::string key)
{
    _accessSemaphore.acquire();

    if(_mem != NULL)
        _mem->erase(key);

    _accessSemaphore.release();

}

MemItem* GlobalMemory::get(std::string key)
{

    _accessSemaphore.acquire();
    if(_mem != NULL) {
        MemItem * item =  _mem->find(key)->second;
        _accessSemaphore.release();
        return item;
    }
    _accessSemaphore.release();
}

void GlobalMemory::deInit() {
	_mem->clear();
	delete _mem;
}

void GlobalMemory::init() {
    //PrintUtil::DebugPrint("why \n");
    _mem = new std::unordered_map<std::string, MemItem* >();
    _accessSemaphore = Semaphore();
    _accessSemaphore.release();
}
