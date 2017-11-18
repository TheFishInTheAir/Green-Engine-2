#pragma once

#define ge_DEBUG_TIMER_INIT auto start = std::chrono::system_clock::now(); auto end = std::chrono::system_clock::now(); std::chrono::duration<double> elapsed_seconds;

#define ge_DEBUG_TIMER_START start = std::chrono::system_clock::now();
#define ge_DEBUG_TIMER_END(m) end = std::chrono::system_clock::now(); elapsed_seconds = end - start; ConsoleIO::print("process name: "+std::string(m)+", elapsed time: " + std::to_string(elapsed_seconds.count()) + "s\n", ge::MessageType::Debug);
