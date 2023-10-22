using System.Net;
using System.Net.Sockets;

using static ipkcpc.ErrorMessages;

namespace ipkcpc;

internal class Connection
{
    private UdpConnector? _udp;
    private TcpConnector? _tcp;
    private Socket _client = default!;

    public Connection(string address, int port, string type)
    {
        var endPoint = ParseIpEndPoint(address, port);
        CreateConnection(endPoint, type);
        SetTimeout(2000);
    }

    private static IPEndPoint ParseIpEndPoint(string ipAddress, int port)
    {
        IPEndPoint endPoint = default!;
        try
        {
            var address = IPAddress.Parse(ipAddress);
            endPoint = new IPEndPoint(address, port);
        }
        catch (FormatException e)
        {
            FatalMessage("Could not parse IP address", e);
        }
        catch (ArgumentOutOfRangeException e)
        {
            FatalMessage("Could not create end point", e);
        }
        return endPoint;
    }

    private void CreateConnection(IPEndPoint endPoint, string type)
    {
        try
        {
            if (type == "udp")
            {
                _udp = new UdpConnector(endPoint);
                _client = _udp.UdpClient.Client;
            }
            else
            {
                _tcp = new TcpConnector(endPoint);
                _client = _tcp.TcpClient.Client;
            }
        }
        catch (SocketException e)
        {
            FatalMessage("Could not create a socket connection", e);
        }
    }

    private void SetTimeout(int timeout)
    {
        _client.ReceiveTimeout = timeout;
        _client.SendTimeout = timeout;
    }

    public void Terminate()
    {
        _udp?.Terminate(); 
        _tcp?.Terminate();
    }

    public dynamic GetConnector()
    {
        dynamic connector = default!;
        if (_udp != null)
        {
            connector =  _udp;
        }
        else if (_tcp != null)
        {
            connector = _tcp;
        }
        return connector;
    }
}
