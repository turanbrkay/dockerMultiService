using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Diagnostics;
using System.Linq;
using System.Threading;

class Program
{
    static void Main()
    {
        var listener = new TcpListener(IPAddress.Any, 5000);
        listener.Start();
        Console.WriteLine("Server started. Listening on port 5000...\n");

        var stopwatch = Stopwatch.StartNew();
        bool waitingMessageShown = false;

        while (stopwatch.Elapsed < TimeSpan.FromMinutes(4))
        {
            if (!listener.Pending())
            {
                if (!waitingMessageShown)
                {
                    Console.WriteLine("Waiting for client...");
                    waitingMessageShown = true;
                }

                Thread.Sleep(500);
                continue;
            }

            waitingMessageShown = false;

            using var client = listener.AcceptTcpClient();
            Console.WriteLine("Client connected.\n"); // 👈 Bağlantıdan sonra boşluk

            using var stream = client.GetStream();

            byte[] buffer = new byte[1024];
            int len = stream.Read(buffer, 0, buffer.Length);
            string input = Encoding.UTF8.GetString(buffer, 0, len);
            Console.WriteLine($"Received: {input}");

            // Sayıları +3 arttır
            var numbers = input.Split(',').Select(int.Parse).Select(x => x + 3);
            string output = string.Join(",", numbers);
            Console.WriteLine($"Processed output: {output}");

            try
            {
                string host = Environment.GetEnvironmentVariable("APP2_HOST");
                int port = int.Parse(Environment.GetEnvironmentVariable("APP2_PORT"));
                Console.WriteLine($"Sending to app2 (UDP) at {host}:{port}...");

                using var udpClient = new UdpClient();
                byte[] outBuffer = Encoding.UTF8.GetBytes(output);
                udpClient.Send(outBuffer, outBuffer.Length, host, port);
                Console.WriteLine("Data sent via UDP.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error sending via UDP: {ex.Message}");
            }


            Console.WriteLine(); // 👈 Gönderimden sonra boşluk
        }

        listener.Stop();
        Console.WriteLine("Server stopped after 2 minutes.");
    }
}
