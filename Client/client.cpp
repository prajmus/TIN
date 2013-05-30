#include "client.h"

Client::Client()
{
}

Client::getInstance()
{
    static Client instance;
    return instance;
}
