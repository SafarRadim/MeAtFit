using System.CommandLine;

namespace ipkcpc;

internal class Program
{
    private static async Task Main(string[] args)
    {
        Connection? connection;

        // START OF ARGPARSE
        var rootCommand = new RootCommand("Client for IPK Calculator Protocol");

        var hostOption = new Option<string>
            (name: "--host",
                description: "IPv4 address of the server") 
            {IsRequired = true};
        hostOption.AddAlias("-h");

        var portOption = new Option<int>
            (name: "--port",
                description: "Server port")
            {IsRequired = true};
        portOption.AddAlias("-p");

        var modeOption = new Option<string>
                (name: "--mode",
                    description: "Connection mode tcp or udp")
                { IsRequired = true }
            .FromAmong("udp", "tcp");
        modeOption.AddAlias("-m");

        rootCommand.AddOption(hostOption);
        rootCommand.AddOption(portOption);
        rootCommand.AddOption(modeOption);

        rootCommand.SetHandler((hostOptionValue, portOptionValue, modeOptionValue) =>
        {
            connection = new Connection(hostOptionValue, portOptionValue, modeOptionValue);
            ProgramLoop(connection);

        }, hostOption, portOption, modeOption);
        // END OF ARGPARSE

        await rootCommand.InvokeAsync(args);
    }

    public static void ProgramLoop(Connection? connection)
    {
        var stop = false;

        if (connection is null)
        {
            Console.Error.WriteLineAsync("[FATAL] Failed to establish connection");
            Environment.Exit(1);
        }

        Console.CancelKeyPress += delegate
        {
            connection.Terminate();
            Console.WriteLine("Terminated gracefully.");
            stop = true;
        };

        var client = connection.GetConnector();

        #if DEBUG
        Console.WriteLine("Ready");
        #endif

        while (!stop)
        {
            var message = Console.ReadLine();
            if (message is null) break;
            var count = client.Send(message);
            if (count == -1) continue;
            message = client.Receive();
            Console.Write(message);
        }

        connection.Terminate();
    }
}