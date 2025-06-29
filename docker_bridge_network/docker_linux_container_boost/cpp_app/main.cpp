#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <chrono>

using boost::asio::ip::udp;

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
    const char *portEnv = std::getenv("APP2_PORT");
    if (!portEnv)
    {
        std::cerr << "[ERROR] APP2_PORT environment variable not set!" << std::endl;
        return 1;
    }
    int port = std::stoi(portEnv);

    boost::asio::io_context io_context;
    udp::socket socket(io_context, udp::endpoint(udp::v4(), port));

    std::cout << "[UDP Server] Listening on port " << port << "..." << std::endl;
    auto start_time = std::chrono::steady_clock::now();

    while (true)
    {
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count() > 120)
        {
            std::cout << "[UDP Server] Time limit reached. Exiting..." << std::endl;
            return 0;
        }

        char buffer[1024];
        udp::endpoint remote_endpoint;
        boost::system::error_code error;

        size_t len = socket.receive_from(boost::asio::buffer(buffer), remote_endpoint, 0, error);

        if (error && error != boost::asio::error::message_size)
        {
            std::cerr << "[ERROR] Receive failed: " << error.message() << std::endl;
            continue;
        }

        std::string input(buffer, len);
        std::cout << "[app1 → udp_server] Received: " << input << std::endl;
    }

    return 0;
}
