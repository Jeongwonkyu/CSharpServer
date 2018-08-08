﻿using System;
using CSharpServer;
using NDesk.Options;

namespace UdpEchoServer
{
    class EchoServer : UdpServer
    {
        public EchoServer(Service service, InternetProtocol protocol, int port) : base(service, protocol, port) {}

        protected override void OnReceived(UdpEndpoint endpoint, byte[] buffer)
        {
            // Echo the message back to the sender
            Send(endpoint, buffer);
        }

        protected override void OnError(int error, string category, string message)
        {
            Console.WriteLine($"Server caught an error with code {error} and category '{category}': {message}");
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            bool help = false;
            int port = 3333;
            int threads = Environment.ProcessorCount;

            var options = new OptionSet()
            {
                { "h|?|help",   v => help = v != null },
                { "p|port=", v => port = int.Parse(v) },
                { "t|threads=", v => threads = int.Parse(v) }
            };

            try
            {
                options.Parse(args);
            }
            catch (OptionException e)
            {
                Console.Write("Command line error: ");
                Console.WriteLine(e.Message);
                Console.WriteLine("Try `--help' to get usage information.");
                return;
            }

            if (help)
            {
                Console.WriteLine("Usage:");
                options.WriteOptionDescriptions(Console.Out);
                return;
            }

            Console.WriteLine($"Server port: {port}");
            Console.WriteLine($"Working threads: {threads}");

            // Create a new service
            var service = new Service(threads);

            // Start the service
            Console.Write("Service starting...");
            service.Start();
            Console.WriteLine("Done!");

            // Create a new echo server
            var server = new EchoServer(service, InternetProtocol.IPv4, port);
            server.SetupReuseAddress(true);
            server.SetupReusePort(true);

            // Start the server
            Console.Write("Server starting...");
            server.Start();
            Console.WriteLine("Done!");

            Console.WriteLine("Press Enter to stop the server or '!' to restart the server...");

            // Perform text input
            for (;;)
            {
                string line = Console.ReadLine();
                if (line == String.Empty)
                    break;

                // Restart the server
                if (line == "!")
                {
                    Console.Write("Server restarting...");
                    server.Restart();
                    Console.WriteLine("Done!");
                }
            }

            // Stop the server
            Console.Write("Server stopping...");
            server.Stop();
            Console.WriteLine("Done!");

            // Stop the service
            Console.Write("Service stopping...");
            service.Stop();
            Console.WriteLine("Done!");
        }
    }
}