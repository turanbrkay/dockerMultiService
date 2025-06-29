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
    std::cout << "[DEBUG] Parsing input: " << input << std::endl;
    std::stringstream ss(input);
    std::string item;
    std::vector<int> result;
    while (std::getline(ss, item, ','))
    {
        try
        {
            result.push_back(std::stoi(item) + 3);
        }
        catch (std::exception &e)
        {
            std::cerr << "[ERROR] parse stoi failed: " << e.what() << std::endl;
        }
    }
    return result;
}

int main()
{
    std::cout << "[INFO] Starting UDP server..." << std::endl;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cerr << "[ERROR] WSAStartup failed." << std::endl;
        return 1;
    }
    std::cout << "[INFO] WSAStartup succeeded." << std::endl;

    // Ortam değişkeninden port oku
    const char *portEnv = std::getenv("APP2_PORT");
    int port = 6000; // default değer
    if (portEnv)
    {
        try
        {
            port = std::stoi(portEnv);
            std::cout << "[INFO] Using port from APP2_PORT: " << port << std::endl;
        }
        catch (std::exception &e)
        {
            std::cerr << "[WARN] APP2_PORT parse failed: " << e.what() << ", using default port: " << port << std::endl;
        }
    }
    else
    {
        std::cout << "[INFO] APP2_PORT not set, using default port: " << port << std::endl;
    }

    SOCKET server = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    if (server == INVALID_SOCKET)
    {
        std::cerr << "[ERROR] socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "[INFO] Socket created." << std::endl;

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        std::cerr << "[ERROR] bind failed." << std::endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    std::cout << "[INFO] Socket bound to port " << port << ", waiting for UDP packets..." << std::endl;

    auto start_time = std::chrono::steady_clock::now();

    while (true)
    {
        auto now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count();
        if (elapsed > 240)
        {
            std::cout << "[INFO] Time limit reached (" << elapsed << "s). Exiting..." << std::endl;
            break;
        }

        char buffer[1024];
        sockaddr_in senderAddr;
        int senderSize = sizeof(senderAddr);
        int len = recvfrom(server, buffer, sizeof(buffer), 0, (sockaddr *)&senderAddr, &senderSize);
        if (len == SOCKET_ERROR)
        {
            std::cerr << "[ERROR] recvfrom failed with code: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::string input(buffer, len);
        std::cout << "[INFO] Received message: \"" << input << "\"" << std::endl;
    }

    std::cout << "[INFO] Closing socket and cleaning up..." << std::endl;
    closesocket(server);
    WSACleanup();
    return 0;
}
