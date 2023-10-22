namespace project.App.Messages;

public record ActivityEditMessage
{
    public required Guid ActivityId { get; init; }
}