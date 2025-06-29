#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>         // close()
#include <cstring>          // memset
#include <cstdlib>          // getenv
#include <chrono>

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
    // Ortam değişkeninden port al
    const char *portEnv = std::getenv("APP2_PORT");
    if (!portEnv)
    {
        std::cerr << "[ERROR] APP2_PORT environment variable not set!" << std::endl;
        return 1;
    }
    int port = std::stoi(portEnv);

    // UDP socket oluştur
    int server = socket(AF_INET, SOCK_DGRAM, 0);
    if (server < 0)
    {
        std::cerr << "[ERROR] Failed to create socket" << std::endl;
        return 1;
    }

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "[ERROR] Bind failed" << std::endl;
        return 1;
    }

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
        socklen_t senderSize = sizeof(senderAddr);
        int len = recvfrom(server, buffer, sizeof(buffer), 0, (sockaddr *)&senderAddr, &senderSize);
        if (len < 0)
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
        std::cout << std::endl;
    }

    close(server);
    return 0;
}
