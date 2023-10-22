namespace ipkcpc;

public static class ErrorMessages
{
    public static void FatalMessage(string message, Exception e)
    {
        Console.Error.WriteLine("[FATAL] " + message);
        #if DEBUG
        Console.Error.WriteLine(e.ToString());
        #endif
        Environment.Exit(1);
    }
    
    public static void WarningMessage(string message)
    {
        Console.Error.WriteLine("[WARNING] " + message);
    }

    public static void WarningMessage(string message, Exception e)
    {
        Console.Error.WriteLine("[WARNING] " + message);
        #if DEBUG
        Console.Error.WriteLine(e.ToString());
        #endif
    }
}
