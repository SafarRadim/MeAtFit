namespace project.DAL.Entities;

public record UserProjectEntity : IEntity
{
    public Guid Id { get; set; }

    public Guid UserId { get; set; }
    public UserEntity? User { get; set; }

    public Guid ProjectId { get; set; }
    public ProjectEntity? Project { get; set; }
}