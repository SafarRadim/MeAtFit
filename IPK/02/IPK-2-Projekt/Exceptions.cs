using System.Net.Sockets;

namespace IPK_2_Projekt;

/// <summary>
/// Thrown when IP address is invalid during subnet parsing.
/// </summary>
public class InvalidIpAddressException : Exception
{
    public InvalidIpAddressException() { }
    public InvalidIpAddressException(string message) : base(message) { }
    public InvalidIpAddressException(string message, Exception inner) : base(message, inner) { }
}

/// <summary>
/// Thrown when prefix is invalid during subnet parsing.
/// </summary>
public class InvalidPrefixException : Exception
{
    public InvalidPrefixException() { }
    public InvalidPrefixException(string message) : base(message) { }
    public InvalidPrefixException(string message, Exception innerException) : base(message, innerException) { }
}

/// <summary>
/// Thrown when interface is not valid during scanning.
/// </summary>
public class InvalidInterfaceException : Exception
{
    public InvalidInterfaceException() { }
    public InvalidInterfaceException(string message) : base(message) { }
    public InvalidInterfaceException(string message, Exception innerException) : base(message, innerException) { }
}

/// <summary>
/// Thrown when prefix value is not supported by this program.
/// </summary>
public class UnsupportedPrefixException : Exception
{
    public UnsupportedPrefixException() { }
    public UnsupportedPrefixException(string message) : base(message) { }
    public UnsupportedPrefixException(string message, Exception innerException) : base(message, innerException) { }
}