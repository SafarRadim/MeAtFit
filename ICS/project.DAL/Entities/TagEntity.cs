namespace project.DAL.Entities;

public record TagEntity : IEntity
{
    public Guid Id { get; set; }

    public required string Name { get; set; }
}