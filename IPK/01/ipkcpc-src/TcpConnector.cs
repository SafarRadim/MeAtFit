using System.Net;
using System.Net.Sockets;
using System.Text;
// ReSharper disable MustUseReturnValue

using static ipkcpc.ErrorMessages;

namespace ipkcpc;

internal class TcpConnector
{
    public TcpClient TcpClient { get; }
    private readonly NetworkStream _stream;
    public TcpConnector(IPEndPoint endpoint)
    {
        TcpClient = new TcpClient();
        TcpClient.Connect(endpoint);

        _stream = TcpClient.GetStream();
    }

    public void Terminate()
    {
        const string outMessage = "BYE\n";
        Console.Write(outMessage);
        Send(outMessage);
        Console.WriteLine(Receive());
        Close();
    }

    private void Close()
    {
        TcpClient.Close();
        WarningMessage("Connection closed");
        Environment.Exit(0);
    }

    public int Send(string message)
    {
        var bytes = Encoding.ASCII.GetBytes(message).ToList();
        bytes.Add(10);
        try
        {
            _stream.Write(bytes.ToArray());
        }
        catch (IOException e)
        {
            WarningMessage("Could not send the message", e);
            return -1;
        }
        return bytes.Count;
    }

    public string Receive()
    {
        var bytes = new byte[1024];
        string message = "";
        while (!message.Contains("\n"))
        {
            bytes = new byte[1024];
            try
            {
                _stream.Read(bytes);
            }
            catch (IOException e)
            {
                WarningMessage("Could not receive the message", e);
                return "";
            }

            message += Encoding.ASCII.GetString(bytes);
        }

        message = message.Replace("\0", string.Empty);

        if (!message.Contains("BYE\n")) return message;
        Console.Write(message); 
        Close();

        return message;
    }
}