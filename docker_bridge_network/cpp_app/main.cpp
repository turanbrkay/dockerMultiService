#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <winsock2.h>
#include <chrono>

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
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(6000);

    bind(server, (sockaddr *)&addr, sizeof(addr));
    std::cout << "UDP Server started on port 6000..." << std::endl;

    auto start_time = std::chrono::steady_clock::now();

    while (true)
    {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count() > 120)
        {
            std::cout << "Time limit reached. Exiting..." << std::endl;
            break;
        }

        char buffer[1024];
        sockaddr_in senderAddr;
        int senderSize = sizeof(senderAddr);
        int len = recvfrom(server, buffer, sizeof(buffer), 0, (sockaddr *)&senderAddr, &senderSize);
        if (len == SOCKET_ERROR)
        {
            std::cerr << "recvfrom failed." << std::endl;
            continue;
        }

        std::string input(buffer, len);
        std::cout << "\nReceived UDP data: " << input << std::endl;
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
