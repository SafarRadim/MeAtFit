using System.CommandLine;
using System.Text;
using SharpPcap.LibPcap;

namespace IPK_2_Projekt;

internal class Program
{
    public static async Task Main(string[] args)
    {
        // Start of Argparse
        var rootCommand =
            new RootCommand("Application that scans the presence of L2 and L3 devices on given network segment(s)");

        var interfaceOption = new Option<string> (
            name: "--interface",
            description: "Which interface to scan on",
            getDefaultValue: () => string.Empty
        ) {
            Arity = ArgumentArity.ZeroOrOne
        };
        interfaceOption.AddAlias("-i");
        rootCommand.AddOption(interfaceOption);


        var timeoutOption = new Option<int> (
            name: "--wait",
            description: "Timeout for a single scan, in ms",
            getDefaultValue: () => 5000
        );
        timeoutOption.AddAlias("-w");
        rootCommand.AddOption(timeoutOption);

        var subnetOption = new Option<string[]> (
            name: "--subnet",
            description: "IPv4/6 subnet to scan, format IP/Mask",
            getDefaultValue: Array.Empty<string>
        ) {
            Arity = ArgumentArity.ZeroOrMore
        };
        subnetOption.AddAlias("-s");
        rootCommand.AddOption(subnetOption);

        rootCommand.SetHandler((interfaceOptionValue, timeoutOptionValue, subnetOptionValue) =>
        {
            if (interfaceOptionValue is null or "")
            {
                PrintInterface();
                Environment.Exit(1);
            }

            if (subnetOptionValue.Length == 0)
            {
                PrintInterface();
                Environment.Exit(1);
            }

            try
            {

                var scanner = new IpkL2L3Scan(interfaceOptionValue, timeoutOptionValue, subnetOptionValue);
                scanner.PrintSubnets();
                scanner.Scan();
            }
            catch (InvalidIpAddressException ex)
            {
                Console.WriteLine(ex.Message);
                Environment.Exit(2);
            }
            catch (InvalidPrefixException ex)
            {
                Console.WriteLine(ex.Message);
                Environment.Exit(3);
            }
            catch (InvalidInterfaceException ex)
            {
                Console.WriteLine(ex.Message);
                Environment.Exit(4);
            }
            catch (UnsupportedPrefixException ex)
            {
                Console.WriteLine(ex.Message);
                Environment.Exit(5);
            }
        }, interfaceOption, timeoutOption, subnetOption);

        // End of Argparse

        await rootCommand.InvokeAsync(args);
        
    }

    /// <summary>
    /// Prints interfaces in human readable format.
    /// </summary>
    private static void PrintInterface()
    {
        var devices = LibPcapLiveDeviceList.Instance;
        foreach (var device in devices)
        {
            var sb = new StringBuilder();
            sb.AppendLine(device.Interface.FriendlyName);
            sb.AppendLine("=========================================");
            sb.AppendLine(device.Interface.Name);
            sb.AppendLine(device.Interface.Description);
            foreach (var address in device.Interface.Addresses)
            {
                sb.AppendLine(address.ToString());
            }

            sb.AppendLine();

            Console.WriteLine(sb.ToString());
        }
    }
}
