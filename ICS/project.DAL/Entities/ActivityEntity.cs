namespace project.DAL.Entities;

public record ActivityEntity : IEntity
{
    public Guid Id { get; set; }
    public Guid ProjectId { get; set; }

    public DateTime Start { get; set; }
    public DateTime End { get; set; }

    public Guid? UserEntityId { get; set; }

    public required string Name { get; set; }
    public required string Description { get; set; }

    public ICollection<TagActivityEntity>? TagActivities { get; set; }
}