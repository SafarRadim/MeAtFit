using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Text;
using PacketDotNet;
using PacketDotNet.Utils;
using SharpPcap;
using SharpPcap.LibPcap;
using ProtocolType = System.Net.Sockets.ProtocolType;

namespace IPK_2_Projekt;

public class IpkL2L3Scan
{
    private readonly SubnetNetwork[] _subnets;
    private readonly ARP _arp;
    private readonly int _timeout;
    private readonly LibPcapLiveDevice _device;
    private readonly IPAddress? _ipv4;
    private readonly IPAddress? _ipv6;

    /// <summary>
    /// Main class of the program.
    /// Does the scan part.
    /// </summary>
    /// <param name="interfaceOption">Interface to scan from</param>
    /// <param name="timeout">Timeout for each part, in ms</param>
    /// <param name="subnetStrings">List of subnets to scan</param>
    public IpkL2L3Scan(string interfaceOption, int timeout, IReadOnlyList<string> subnetStrings)
    {
        _subnets = GetSubnets(subnetStrings);

        _device = GetDevice(interfaceOption);
        _ipv4 = GetIp(4);
        _ipv6 = GetIp(6);


        _arp = new ARP(_device)
        {
            Timeout = TimeSpan.FromMilliseconds(timeout)
        };

        _timeout = timeout;
    }

    /// <summary>
    /// Calls the proper scan method on each subnet.
    /// </summary>
    public void Scan()
    {
        foreach (var subnet in _subnets)
        {
            if (subnet.Version == 4)
            {
                ScanV4(subnet);
            }
            else
            {
                ScanV6(subnet);
            }
        }
    }
    /// <summary>
    /// Prints subnets, their prefix and the amount of hosts scanned.
    /// </summary>
    public void PrintSubnets()
    {
        Console.WriteLine("Scanning ranges:");
        foreach (var subnet in _subnets)
        {
            Console.Write(subnet.Ip + "/");
            Console.Write(subnet.Prefix);

            Console.WriteLine(" (" + subnet.Hosts + " hosts)");
        }
    }

    /// <summary>
    /// Gets device set by interface option
    /// </summary>
    /// <param name="interfaceOption"></param>
    /// <returns>Device</returns>
    /// <exception cref="InvalidInterfaceException">If device is not found.</exception>
    private static LibPcapLiveDevice GetDevice(string interfaceOption)
    {
        var devices = CaptureDeviceList.Instance;
        foreach (var device in devices)
        {
            if (device.Name == interfaceOption)
            {
                return (LibPcapLiveDevice)device;
            }
        }

        throw new InvalidInterfaceException();
    }

    /// <summary>
    /// Gets subnets from strings
    /// </summary>
    /// <param name="subnetStrings"></param>
    /// <returns>Array of subnets</returns>
    private static SubnetNetwork[] GetSubnets(IReadOnlyList<string> subnetStrings)
    {
        var subnetArr = new SubnetNetwork[subnetStrings.Count];
        for (var i = 0; i < subnetStrings.Count; i++)
        {
            subnetArr[i] = new SubnetNetwork(subnetStrings[i]);
        }

        return subnetArr;
    }

    /// <summary>
    /// Gets interface IP address.
    /// </summary>
    /// <param name="version">Version, either 4 or 6</param>
    /// <returns>IP address of subnet</returns>
    private IPAddress? GetIp(int version)
    {
        foreach (var address in _device.Addresses)
        {
            if (address.Addr.type == Sockaddr.AddressTypes.AF_INET_AF_INET6)
            {
                switch (address.Addr.ipAddress.AddressFamily)
                {
                    case AddressFamily.InterNetwork when version == 4:
                        return address.Addr.ipAddress;
                    case AddressFamily.InterNetworkV6 when version == 6:
                        return address.Addr.ipAddress;
                }
            }
        }
        return null;
    }

    /// <summary>
    /// Main scan loop for IPv4 addresses.
    /// </summary>
    /// <param name="subnet">Subnet to scan on</param>
    /// <exception cref="InvalidInterfaceException">If device does not have IPv4 address.</exception>
    private void ScanV4(SubnetNetwork subnet)
    {
        var hostAddress = SubnetNetwork.GetNextIpAddress(subnet.Ip);
        if (_ipv4 == null)
        {
            throw new InvalidInterfaceException("No IPv4 address found for interface - Cannot scan on IPv4");
        }

        for (ulong i = 0; i < subnet.Hosts; i++)
        {
            
            var outString = new StringBuilder(hostAddress.ToString(), 128);

            outString.Append("\tarp");
            var macAddress = _arp.Resolve(hostAddress);
            if (macAddress != null)
            {
                outString.Append(" OK");
                outString.Append(" (" + FormatMac(macAddress.ToString()) + ")");
            }
            else
            {
                outString.Append(" FAIL");
            }

            outString.Append(", icmpv4");

            var icmp = GetIcmpV4(_ipv4, hostAddress);

            var localEndPoint = new IPEndPoint(_ipv4, 0);
            var socket = new Socket(AddressFamily.InterNetwork, SocketType.Raw, ProtocolType.Icmp)
            {
                ReceiveTimeout = _timeout
            };
            socket.Bind(localEndPoint);
            
            var remoteEndpoint = new IPEndPoint(hostAddress, 0);
            var buffer = new byte[1024];

            socket.Connect(remoteEndpoint);
            socket.Send(icmp.Bytes);
            try
            {
                socket.Receive(buffer);
            }
            catch (SocketException ex)
            {
                if (ex.SocketErrorCode != SocketError.NetworkUnreachable)
                {
                    
                }
                // ICMP failed -> IP not responding
                socket.Shutdown(SocketShutdown.Both);
                outString.Append(" FAIL");
                Console.WriteLine(outString.ToString());
                hostAddress = SubnetNetwork.GetNextIpAddress(hostAddress);
                continue;
            }

            socket.Shutdown(SocketShutdown.Both);

            outString.Append(buffer[20] == 0 ? " OK" : " FAIL");

            Console.WriteLine(outString.ToString());

            hostAddress = SubnetNetwork.GetNextIpAddress(hostAddress);
        }
    }

    /// <summary>
    /// Black magic to construct proper ICMPv4 packet.
    /// </summary>
    /// <param name="source">Source IP</param>
    /// <param name="dest">Destination IP</param>
    /// <returns>Constructed packet</returns>
    private static IcmpV4Packet GetIcmpV4(IPAddress source, IPAddress dest)
    {
        var tempPacket = new IPv4Packet(source, dest);
        var bytes = tempPacket.HeaderSegment;


        var icmpPacket = new IcmpV4Packet(bytes)
        {
            TypeCode = IcmpV4TypeCode.EchoRequest
        };
        icmpPacket.UpdateIcmpChecksum();
        return icmpPacket;
    }

    /// <summary>
    /// Formats mac address with ":" in between bytes
    /// </summary>
    /// <param name="mac">Mac address to format.</param>
    /// <returns>String of formatted address</returns>
    private static string FormatMac(string mac)
    {
        var newMac = new StringBuilder(32);

        for (var i = 0; i < mac.Length; i++)
        {
            newMac.Append(mac[i]);
            if (i % 2 == 1 && i != mac.Length - 1) newMac.Append(':');
        }

        return newMac.ToString();
    }

    /// <summary>
    /// Main scan loop for IPv6
    /// </summary>
    /// <param name="subnet">Subnet to scan on</param>
    /// <remarks>NDP is a bit iffy, not a single clue why.</remarks>
    /// <exception cref="InvalidInterfaceException">If device does not have IPv6 address.</exception>
    private void ScanV6(SubnetNetwork subnet)
    {
        var hostAddress = SubnetNetwork.GetNextIpAddress(subnet.Ip);
        if (_ipv6 == null)
        {
            throw new InvalidInterfaceException("No IPv6 address found for interface - Cannot scan on IPv6");
        }

        for (ulong i = 0; i < subnet.Hosts; i++)
        {
            var outString = new StringBuilder(hostAddress.ToString(), 128);
            outString.Append("\t NDP");
            var socket = new Socket(AddressFamily.InterNetworkV6, SocketType.Raw, ProtocolType.IcmpV6)
            {
                ReceiveTimeout = _timeout
            };
            var localEndPoint = new IPEndPoint(_ipv6, 0); 
            var remoteEndPoint = new IPEndPoint(hostAddress, 0);
            var ndp = GetNdp(hostAddress, _device.MacAddress);

            var err = false;


            socket.Bind(localEndPoint);
            socket.Connect(remoteEndPoint);
            socket.Send(ndp.ToArray());

            var buff = new byte[1024];
            try
            {
                socket.Receive(buff);
            }
            catch (Exception)
            {
                err = true;
            }

            socket.Shutdown(SocketShutdown.Both);

            outString.Append(err ? " FAIL" : " OK");
            err = false;

            outString.Append(", icmpv6");

            var socketIcmp = new Socket(AddressFamily.InterNetworkV6, SocketType.Raw, ProtocolType.IcmpV6)
            {
                ReceiveTimeout = _timeout
            };
            socketIcmp.Bind(localEndPoint);
            socketIcmp.Connect(remoteEndPoint);
            var icmp = GetIcmpv6();
            socketIcmp.Send(icmp.ToArray());

            buff = new byte[128];
            try
            {
                socketIcmp.Receive(buff);
            }
            catch (SocketException)
            {
                err = true;
            }

            if (err)
            {
                outString.Append(" FAIL");
                socketIcmp.Shutdown(SocketShutdown.Both);
                Console.WriteLine(outString.ToString());
                hostAddress = SubnetNetwork.GetNextIpAddress(hostAddress);
                continue;
            }

            var response = new IcmpV6Packet(new ByteArraySegment(buff));
            outString.Append(response.Type == IcmpV6Type.EchoReply ? " OK" : " FAIL");
            socketIcmp.Shutdown(SocketShutdown.Both);

            Console.WriteLine(outString.ToString());
            hostAddress = SubnetNetwork.GetNextIpAddress(hostAddress);
        }
    }

    /// <summary>
    /// Black magic to construct NDP packet
    /// </summary>
    /// <param name="destination">Destination IP address</param>
    /// <param name="sourceMac">Source MAC address</param>
    /// <returns>NDP packet as a IEnumerable of bytes</returns>
    private static IEnumerable<byte> GetNdp(IPAddress destination, PhysicalAddress sourceMac)
    {
        var ndp = new List<byte>(30)
        {
            135,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        };

        ndp.AddRange(destination.GetAddressBytes());
        ndp.Add(1);
        ndp.Add(1);
        ndp.AddRange(sourceMac.GetAddressBytes());

        return ndp.ToArray();
    }

    /// <summary>
    /// Black magic to construct ICMPv6 Echo request packet
    /// </summary>
    /// <returns>ICMPv6 Echo request packet as IEnumerable of bytes</returns>
    private static IEnumerable<byte> GetIcmpv6()
    {
        var icmp = new List<byte>(30)
        {
            128,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        };
        return icmp.ToArray();
    }
}