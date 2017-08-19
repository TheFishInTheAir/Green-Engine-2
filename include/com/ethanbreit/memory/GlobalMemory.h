#pragma once
#include <unordered_map>
#include <string>
#include <Templates/MemItem.h>
#include <Thread/Semaphore.h>

class GlobalMemory
{
public:
    static void insert(std::string, MemItem*);
	static void remove(std::string);
	static MemItem* get(std::string);
	static void deInit();
    static void init();

private:

	static std::unordered_map<std::string, MemItem* >* _mem;


    static Semaphore _accessSemaphore;
};