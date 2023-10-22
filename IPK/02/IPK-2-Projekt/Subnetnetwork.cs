using System.Collections;
using System.Net;
using System.Net.Sockets;

namespace IPK_2_Projekt;

public class SubnetNetwork
{
    /// <summary>
    /// Network IP of subnet
    /// </summary>
    public IPAddress Ip { get; }

    /// <summary>
    /// Max amount of hosts on subnet
    /// </summary>
    public ulong Hosts { get; }

    /// <summary>
    /// Subnet IP version, either 4 or 6
    /// </summary>
    public int Version { get; }
    
    /// <summary>
    /// Prefix of subnet
    /// </summary>
    public int Prefix { get; }

    private readonly byte[] _mask;

    /// <summary>
    /// Class responsible for holding and computing subnet information.
    /// </summary>
    /// <param name="ip">Ip of subnet as a string with prefix</param>
    /// <exception cref="InvalidIpAddressException">Thrown if address cannot be parsed.</exception>
    /// <exception cref="InvalidPrefixException">Thrown if prefix is not valid for IP version.</exception>
    public SubnetNetwork(string ip)
    {
        var splitIp = ip.Split('/');

        if (splitIp.Length != 2) throw new InvalidIpAddressException("Address needs mask! " + Ip);

        if (IPAddress.TryParse(splitIp[0], out var outIp))
        {
            Ip = outIp;
        }
        else
        {
            throw new InvalidIpAddressException("Could not parse the IP! " + ip);
        }

        if (!int.TryParse(splitIp[1], out var prefixTmp)) throw new InvalidPrefixException("Mask needs to be int! " + Ip);
        
        Prefix = prefixTmp;
        _mask = ParseMask();
        Hosts = GetHosts();
        Version = GetVersion();

        Ip = GetNetworkIp();
    }

    /// <summary>
    /// Gets next IP address, overflow goes to 0.
    /// </summary>
    /// <param name="ipAddress">IPAddress to increase by one.</param>
    /// <returns>Next IP address</returns>
    public static IPAddress GetNextIpAddress(IPAddress ipAddress)
    {
        var addressBytes = ipAddress.GetAddressBytes().Reverse().ToArray();
        if (ipAddress.AddressFamily == AddressFamily.InterNetwork)
        {
            var ipUint = BitConverter.ToUInt32(addressBytes, 0);
            var nextAddress = BitConverter.GetBytes(ipUint + 1).Reverse().ToArray();
            return new IPAddress(nextAddress);
        }
        else
        {
            // (u)int128 is available in .net7, but with .net6 I am stuck with this
            var upper = BitConverter.ToUInt64(addressBytes, 8);
            var lower = BitConverter.ToUInt64(addressBytes, 0);

            var upperAddress = BitConverter.GetBytes(upper).Reverse().ToArray();
            var lowerAddress = BitConverter.GetBytes(lower + 1).Reverse().ToArray();

            var nextAddress = upperAddress.Concat(lowerAddress).ToArray();
            return new IPAddress(nextAddress);
        }
    }

    /// <summary>
    /// Creates mask from prefix
    /// </summary>
    /// <returns>Mask as byte array</returns>
    /// <exception cref="InvalidPrefixException">Thrown if prefix is not valid for IP.</exception>
    /// <exception cref="UnsupportedPrefixException">Thrown if prefix value is not supported.</exception>
    private byte[] ParseMask()
    {
        switch (Prefix)
        {
            case 32 when Ip.AddressFamily == AddressFamily.InterNetwork:
                throw new UnsupportedPrefixException("Prefix value not supported. " + Ip);
            case > 31 or < 0 when Ip.AddressFamily == AddressFamily.InterNetwork:
                throw new InvalidPrefixException("Prefix value not valid for IPv4! " + Ip);
            case < 0 or > 128 when Ip.AddressFamily == AddressFamily.InterNetworkV6:
                throw new InvalidPrefixException("Prefix value not valid for IPv4! " + Ip);
            case < 64 when Ip.AddressFamily == AddressFamily.InterNetworkV6:
                throw new UnsupportedPrefixException("Prefix value not supported. " + Ip);
        }

        var mask = Ip.AddressFamily == AddressFamily.InterNetwork ? new BitArray(32) : new BitArray(128);

        for (var i = 0; i < Prefix; i++)
        {
            mask.Set(i, true);
        }
        
        var maskArray = new byte[(mask.Length - 1) / 8 + 1];

        for (var i = 0; i < maskArray.Length; i++)
        {
            var temp = new BitArray(8);
            for (var j = 0; j < 8; j++)
            {
                temp.Set(7 - j, mask.Get(8 * i + j));
            }
            temp.CopyTo(maskArray, i);
        }

        return maskArray;
    }

    /// <summary>
    /// Gets maximum amount of hosts for subnet.
    /// </summary>
    /// <returns>Amount of hosts</returns>
    /// <exception cref="InvalidPrefixException">Thrown if prefix is not valid for IP.</exception>
    /// <exception cref="UnsupportedPrefixException">Thrown if prefix value is not supported.</exception>
    private ulong GetHosts()
    {
        switch (Prefix)
        {
            case 32 when Ip.AddressFamily == AddressFamily.InterNetwork:
                throw new UnsupportedPrefixException("Prefix value not supported. " + Ip);
            case > 31 or < 0 when Ip.AddressFamily == AddressFamily.InterNetwork:
                throw new InvalidPrefixException("Prefix value not valid for IPv4! " + Ip);
            case < 0 or > 128 when Ip.AddressFamily == AddressFamily.InterNetworkV6:
                throw new InvalidPrefixException("Prefix value not valid for IPv4! " + Ip);
            case < 64 when Ip.AddressFamily == AddressFamily.InterNetworkV6:
                throw new UnsupportedPrefixException("Prefix value not supported. " + Ip);
        }

        ulong hosts;

        if (Ip.AddressFamily == AddressFamily.InterNetwork)
        {
            hosts = (ulong)(Math.Pow(2, 32 - Prefix) - 2);
        }
        else
        {
            hosts = (ulong)(Math.Pow(2, (128 - Prefix)));
        }

        return hosts;
    }

    /// <summary>
    /// Gets network IP from string (passed as a host).
    /// </summary>
    /// <returns>IP address of network</returns>
    private IPAddress GetNetworkIp()
    {
        var temp = new byte[Ip.GetAddressBytes().Length];
        var ipBytes = Ip.GetAddressBytes();
        for (var i = 0; i <= Math.DivRem(Prefix, 8, out _); i++)
        {
            if (i == 16) break;
            temp[i] = (byte)(ipBytes[i] & _mask[i]);
        }

        return new IPAddress(temp);
    }

    /// <summary>
    /// Gets IP version, either 4 or 6.
    /// </summary>
    /// <returns>IP version</returns>
    private int GetVersion()
    {
        return Ip.AddressFamily == AddressFamily.InterNetwork ? 4 : 6;
    }
}