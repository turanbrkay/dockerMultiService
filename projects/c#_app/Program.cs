using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Diagnostics;
using System.Linq;

class Program
{
    static void Main()
    {
        var listener = new TcpListener(IPAddress.Parse("192.168.1.242"), 5000);

        listener.Start();
        Console.WriteLine("Server started. Listening on port 7500...");

        var stopwatch = Stopwatch.StartNew();

        while (stopwatch.Elapsed < TimeSpan.FromMinutes(1))
        {
            if (!listener.Pending())
            {
                Console.WriteLine("Waiting for client...");
                Thread.Sleep(500);
                continue;
            }

            using var client = listener.AcceptTcpClient();
            Console.WriteLine("Client connected.");
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
                // ENV'den al: APP2_HOST, APP2_PORT
                string host = Environment.GetEnvironmentVariable("APP2_HOST");
                int port = int.Parse(Environment.GetEnvironmentVariable("APP2_PORT"));

                Console.WriteLine($"Connecting to app2 at {host}:{port}...");

                using var client2 = new TcpClient(host, port);
                using var stream2 = client2.GetStream();
                byte[] outBuffer = Encoding.UTF8.GetBytes(output);
                stream2.Write(outBuffer, 0, outBuffer.Length);
                Console.WriteLine("Data sent to app2.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error sending to app2: {ex.Message}");
            }
        }

        listener.Stop();
        Console.WriteLine("Server stopped after 1 minute.");
    }
}
