using System.Net;
using IPK_2_Projekt;

namespace IPK_2_Projekt_Tests;

public class ParsingIPv4
{
    public class SubnetValid
    {
        private readonly SubnetNetwork _subnet;

        public SubnetValid()
        {
            _subnet = new SubnetNetwork("192.168.0.5/25");
        }

        [Fact]
        public void Ip()
        {
            var expectedIp = IPAddress.Parse("192.168.0.0");
            Assert.Equal(expectedIp, _subnet.Ip);
        }

        [Fact]
        public void Hosts()
        {
            const uint expectedHosts = 126;
            Assert.Equal(expectedHosts, _subnet.Hosts);
        }

        [Fact]
        public void HostsMax()
        {
            var subnet = new SubnetNetwork("192.168.0.5/0");
            const uint expectedHostsMax = uint.MaxValue - 1; // 2^32 - Broadcast and Network
            Assert.Equal(expectedHostsMax, subnet.Hosts);
        }

        [Fact]
        public void HostMin()
        {
            var subnet = new SubnetNetwork("192.168.0.5/31");
            const uint expectedHostMin = 0;
            Assert.Equal(expectedHostMin, subnet.Hosts);
        }

        [Fact]
        public void Prefix()
        {
            const int expectedPrefix = 25;
            Assert.Equal(expectedPrefix, _subnet.Prefix);
        }
    }

    public class SubnetInvalid
    {
        public SubnetNetwork? Subnet { get; private set; }

        [Fact]
        public void MissingPrefix()
        {
            Assert.Throws<InvalidIpAddressException>(() =>
                Subnet = new SubnetNetwork("192.168.0.1")
            );
        }

        [Fact]
        public void MissingIp()
        {
            Assert.Throws<InvalidIpAddressException>(() =>
                Subnet = new SubnetNetwork("/25")
            );
        }

        [Fact]
        public void InvalidIp()
        {
            Assert.Throws<InvalidIpAddressException>(() =>
                Subnet = new SubnetNetwork("192.168.300.1/25")
            );
        }

        [Fact]
        public void UnsupportedPrefix()
        {
            Assert.Throws<UnsupportedPrefixException>(() => 
                Subnet = new SubnetNetwork("192.168.0.1/32")
            );
        }

        [Fact]
        public void InvalidPrefixTooHigh()
        {
            Assert.Throws<InvalidPrefixException>(() =>
                Subnet = new SubnetNetwork("192.168.0.1/33")
            );
        }

        [Fact]
        public void InvalidPrefixTooLow()
        {
            Assert.Throws<InvalidPrefixException>(() =>
                Subnet = new SubnetNetwork("192.168.0.1/-1")
            );
        }
    }
}

public class Ipv4
{
    [Fact]
    public void NextIp()
    {
        var ip = IPAddress.Parse("192.168.0.1");
        var nextIp = IPAddress.Parse("192.168.0.2");
        Assert.Equal(nextIp, SubnetNetwork.GetNextIpAddress(ip));
    }

    [Fact]
    public void NextIpOverByte()
    {
        var ip = IPAddress.Parse("192.168.0.255");
        var nextIp = IPAddress.Parse("192.168.1.0");
        Assert.Equal(nextIp, SubnetNetwork.GetNextIpAddress(ip));
    }

    [Fact]
    public void NextIpOverflow()
    {
        var ip = IPAddress.Parse("255.255.255.255");
        var nextIp = IPAddress.Parse("0.0.0.0");
        Assert.Equal(nextIp, SubnetNetwork.GetNextIpAddress(ip));
    }
}