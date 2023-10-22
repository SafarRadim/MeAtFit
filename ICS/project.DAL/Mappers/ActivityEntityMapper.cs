using project.DAL.Entities;

namespace project.DAL.Mappers;

public class ActivityEntityMapper : IEntityMapper<ActivityEntity>
{
    public void MapToExistingEntity(ActivityEntity existingEntity, ActivityEntity newEntity)
    {
        existingEntity.Id = newEntity.Id;
        existingEntity.ProjectId = newEntity.ProjectId;
        existingEntity.Name = newEntity.Name;
        existingEntity.Description = newEntity.Description;
        existingEntity.Start = newEntity.Start;
        existingEntity.End = newEntity.End;
    }
}