using System.Net;
using System.Net.Sockets;
using System.Text;

using static ipkcpc.ErrorMessages;

namespace ipkcpc;

internal class UdpConnector
{
    public UdpClient UdpClient { get; private set; }
    private IPEndPoint _endpoint;

    public UdpConnector(IPEndPoint endpoint)
    {
        UdpClient = new UdpClient();
        UdpClient.Connect(endpoint);

        _endpoint = endpoint;
    }

    public void Terminate()
    {
       UdpClient.Close();
       Console.WriteLine("Connection closed");
       Environment.Exit(0);
    }

    public int Send(string message)
    {
        if (message.Length > 255)
        {
            WarningMessage("Message is longer than maxlength (255)");
            return -1;
        }
        var bytes = Encoding.ASCII.GetBytes(message).ToList();
        bytes.Insert(0, 0);
        bytes.Insert(1, (byte)message.Length);
        try
        {
            UdpClient.Send(bytes.ToArray());
        }
        catch (SocketException e)
        {
            WarningMessage("Could not send the message", e);
            return -1;
        }

        return bytes.Count;
    }

    public string Receive()
    {
        byte[] bytes;
        try
        {
            bytes = UdpClient.Receive(ref _endpoint);
        }
        catch (SocketException e)
        {
            WarningMessage("Could not receive the message", e);
            return "";
        }
        var offset = CheckResponse(bytes);
        
        var msgLen = bytes.Length - 3 + offset;
        var message = Encoding.ASCII.GetString(bytes, 3, msgLen);
        if (offset == 0)
        {
            return "OK:" + message + "\n";
        }
        else
        {
            return "ERR:" + message + "\n";
        }
    }

    private int CheckResponse(byte[] bytes)
    {
        if (bytes[0] == 0)
        {
            WarningMessage("Received a request instead of response");
            return -1;
        }

        if (bytes[1] == 1)
        {
            WarningMessage("Received a error response");
            return -1;
        }
        return 0;
    }
}