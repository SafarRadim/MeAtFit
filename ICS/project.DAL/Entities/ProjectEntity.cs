namespace project.DAL.Entities;

public record ProjectEntity : IEntity
{
    public Guid Id { get; set; }

    public required string Name { get; set; }
    public required string Description { get; set; }

    public ICollection<UserProjectEntity>? UserProjects { get; set; }
    public ICollection<ActivityEntity>? Activities { get; set; }
}