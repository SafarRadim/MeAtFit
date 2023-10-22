using project.DAL.Entities;

namespace project.DAL.Mappers;

public class UserProjectEntityMapper : IEntityMapper<UserProjectEntity>
{
    public void MapToExistingEntity(UserProjectEntity existingEntity, UserProjectEntity newEntity)
    {
        existingEntity.Id = newEntity.Id;
        existingEntity.UserId = newEntity.UserId;
        existingEntity.ProjectId = newEntity.ProjectId;
    }
}
