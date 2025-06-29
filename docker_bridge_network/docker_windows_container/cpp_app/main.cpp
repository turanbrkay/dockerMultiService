#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <winsock2.h>
#include <chrono>
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
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // ⬇️ Ortam değişkeninden portu al
    const char *portEnv = std::getenv("APP2_PORT");
    int port = std::stoi(portEnv);

    SOCKET server = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port); // ⬅️ Port burada artık dinamik

    bind(server, (sockaddr *)&addr, sizeof(addr));
    std::cout << "[UDP Server] Listening on port " << port << "..." << std::endl;

    auto start_time = std::chrono::steady_clock::now();

    while (true)
    {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count() > 240)
        {
            std::cout << "[UDP Server] Time limit reached. Exiting..." << std::endl;
            break;
        }

        char buffer[1024];
        sockaddr_in senderAddr;
        int senderSize = sizeof(senderAddr);
        int len = recvfrom(server, buffer, sizeof(buffer), 0, (sockaddr *)&senderAddr, &senderSize);
        if (len == SOCKET_ERROR)
        {
            std::cerr << "[UDP Server] recvfrom failed." << std::endl;
            continue;
        }

        std::string input(buffer, len);
        std::cout << "[app1 → udp_server] Received: " << input << std::endl;

        auto updated = parse(input);
        std::string output;
        for (size_t i = 0; i < updated.size(); ++i)
        {
            output += std::to_string(updated[i]);
            if (i != updated.size() - 1)
                output += ",";
        }

        std::cout << "[udp_server] Processed (+3): " << output << std::endl;
        std::cout << std::endl; // Gönderim sonrası boşluk
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
