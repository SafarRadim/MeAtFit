using project.DAL.Entities;

namespace project.DAL.Mappers;

public class TagEntityMapper : IEntityMapper<TagEntity>
{
    public void MapToExistingEntity(TagEntity existingEntity, TagEntity newEntity)
    {
        existingEntity.Id = newEntity.Id;
        existingEntity.Name = newEntity.Name;
    }
}
