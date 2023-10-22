namespace project.DAL.Entities;

public record TagActivityEntity : IEntity
{
    public Guid Id { get; set; }

    public Guid TagId { get; set; }
    public TagEntity? Tag { get; set; }

    public Guid ActivityId { get; set; }
    public ActivityEntity? Activity { get; set; }
}