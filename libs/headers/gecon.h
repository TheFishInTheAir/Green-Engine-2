#pragma once
#include <json/json.hpp>

#define GECON_SUCCESS 0
#define GECON_MESSAGE 1
#define GECON_WARNING 2
#define GECON_ERROR 3
namespace GECON
{
    

    void startNodeChild();
    void end();

    void msg(std::string message, int level);


    void ping(std::string);
}