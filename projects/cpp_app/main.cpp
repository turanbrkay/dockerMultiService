#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <winsock2.h>
#include <chrono>  // süre kontrolü için
#include <cstdlib> // getenv için
#pragma comment(lib, "ws2_32.lib")

std::vector<int> parse(const std::string &input)
{
    std::stringstream ss(input);
    std::string item;
    std::vector<int> result;
    while (std::getline(ss, item, ','))
    {
        result.push_back(std::stoi(item) + 3);
    }
    return result;
}

int main()
{
    WSADATA wsa;
    std::cout << "Initializing Winsock..." << std::endl;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(6000);
    bind(server, (sockaddr *)&addr, sizeof(addr));
    listen(server, 3);
    std::cout << "Listening on port 6000..." << std::endl;

    auto start_time = std::chrono::steady_clock::now();

    while (true)
    {
        // Süre kontrolü: 1 dakika geçtiyse çık
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count() > 60)
        {
            std::cout << "Time limit reached (1 minute). Exiting...\n";
            break;
        }

        std::cout << "\nWaiting for connection..." << std::endl;
        SOCKET client = accept(server, nullptr, nullptr);
        if (client == INVALID_SOCKET)
        {
            std::cerr << "Accept failed.\n";
            continue;
        }

        std::cout << "Client connected!" << std::endl;

        char buf[1024];
        int len = recv(client, buf, sizeof(buf), 0);
        std::string input(buf, len);
        std::cout << "Received data: " << input << std::endl;

        auto updated = parse(input);
        std::string output;
        for (size_t i = 0; i < updated.size(); ++i)
        {
            output += std::to_string(updated[i]);
            if (i != updated.size() - 1)
                output += ",";
        }

        std::cout << "Processed data (each +3): " << output << std::endl;

        // ENV'den app_main bilgilerini al
        const char *hostEnv = std::getenv("APP_MAIN_HOST");
        const char *portEnv = std::getenv("APP_MAIN_PORT");

        std::string host = hostEnv ? hostEnv : "host.docker.internal";
        int port = portEnv ? std::stoi(portEnv) : 7000;

        SOCKET outSock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in mainAddr;
        mainAddr.sin_family = AF_INET;
        mainAddr.sin_port = htons(port);
        mainAddr.sin_addr.s_addr = inet_addr(host.c_str());

        std::cout << "Connecting to app_main at " << host << ":" << port << "..." << std::endl;
        int result = connect(outSock, (sockaddr *)&mainAddr, sizeof(mainAddr));
        if (result == SOCKET_ERROR)
        {
            std::cerr << "Failed to connect to app_main." << std::endl;
        }
        else
        {
            std::cout << "Connected to app_main. Sending data..." << std::endl;
            send(outSock, output.c_str(), output.size(), 0);
            std::cout << "Data sent to app_main." << std::endl;
        }

        closesocket(outSock);
        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
