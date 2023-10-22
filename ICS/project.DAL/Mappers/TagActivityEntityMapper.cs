using project.DAL.Entities;

namespace project.DAL.Mappers;

public class TagActivityEntityMapper : IEntityMapper<TagActivityEntity>
{
    public void MapToExistingEntity(TagActivityEntity existingEntity, TagActivityEntity newEntity)
    {
        existingEntity.ActivityId = newEntity.ActivityId;
        existingEntity.TagId = newEntity.TagId;
    }
}
