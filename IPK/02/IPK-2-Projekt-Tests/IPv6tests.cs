using System.Net;
using IPK_2_Projekt;

namespace IPK_2_Projekt_Tests;

public class ParsingIPv6
{
    public class SubnetValid
    {
        private readonly SubnetNetwork _subnet;

        public SubnetValid()
        {
            _subnet = new SubnetNetwork("fe80::215:5dff:fe99:e01b/96");
        }

        [Fact]
        public void Ip()
        {
            var expectedIp = IPAddress.Parse("fe80::215:5dff:0:0");
            Assert.Equal(expectedIp, _subnet.Ip);
        }

        [Fact]
        public void Hosts()
        {
            const ulong expectedHosts = (ulong)uint.MaxValue + 1;
            Assert.Equal(expectedHosts, _subnet.Hosts);
        }

        [Fact]
        public void HostsMax()
        {
            // Idk why anyone would do this
            // If one IP would take you 1ms, all 2^64 would take around 5.93e8 YEARS.
            const ulong expectedHosts = ulong.MaxValue;
            var subnet = new SubnetNetwork("fe80::/64");
            Assert.Equal(expectedHosts, subnet.Hosts - 1); // a bit of a hack
        }

        [Fact]
        public void HostsMin()
        {
            const ulong expectedHosts = 1;
            var subnet = new SubnetNetwork("fe80::/128");
            Assert.Equal(expectedHosts, subnet.Hosts);
        }

        [Fact]
        public void Prefix()
        {
            const int expectedPrefix = 96;
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
                Subnet = new SubnetNetwork("fe80::")
            );
        }

        [Fact]
        public void MissingIp()
        {
            Assert.Throws<InvalidIpAddressException>(() =>
                Subnet = new SubnetNetwork("/80")
            );
        }

        [Fact]
        public void InvalidIp()
        {
            Assert.Throws<InvalidIpAddressException>(() =>
                Subnet = new SubnetNetwork("mame:radi:ipk:<3::/25")
            );
        }

        [Fact]
        public void UnsupportedPrefix()
        {
            Assert.Throws<UnsupportedPrefixException>(() =>
                Subnet = new SubnetNetwork("fe80::/5")
            );
        }

        [Fact]
        public void InvalidPrefixTooHigh()
        {
            Assert.Throws<InvalidPrefixException>(() =>
                Subnet = new SubnetNetwork("fe80::/129")
            );
        }

        [Fact]
        public void InvalidPrefixTooLow()
        {
            Assert.Throws<InvalidPrefixException>(() =>
                Subnet = new SubnetNetwork("fe80::/-1")
            );
        }
    }
}

public class Ipv6
{
    [Fact]
    public void NextIp()
    {
        var ip = IPAddress.Parse("fe80::2");
        var nextIp = IPAddress.Parse("fe80::3");
        Assert.Equal(nextIp, SubnetNetwork.GetNextIpAddress(ip));
    }

    [Fact]
    public void NextIpOverOctet()
    {
        var ip = IPAddress.Parse("fe80::ffff");
        var nextIp = IPAddress.Parse("fe80::1:0");
        Assert.Equal(nextIp, SubnetNetwork.GetNextIpAddress(ip));
    }

    [Fact]
    public void NextIpOverflow()
    {
        var ip = IPAddress.Parse("fe80::ffff:ffff:ffff:ffff");
        var nextIp = IPAddress.Parse("fe80::");
        Assert.Equal(nextIp, SubnetNetwork.GetNextIpAddress(ip));
    }
}