#ifndef CLIENT_H
#define CLIENT_H

class Client
{
protected:
    Client();
public:
    static Client& getInstance();
    void showStatus();
    void connectToServer();
};

#endif // CLIENT_H
