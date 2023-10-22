using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public class TagModelMapper : ModelMapperBase<TagEntity, TagListModel, TagDetailModel>,
    ITagModelMapper
{
    public override TagListModel MapToListModel(TagEntity? entity)
        => entity is null
            ? TagListModel.Empty
            : new TagListModel { Id = entity.Id, TagName = entity.Name };

    public override TagDetailModel MapToDetailModel(TagEntity? entity)
        => entity is null
            ? TagDetailModel.Empty
            : new TagDetailModel
            {
                Id = entity.Id,
                TagName = entity.Name,
            };

    public override TagEntity MapToEntity(TagDetailModel model)
        => new() { Id = model.Id, Name = model.TagName };
}
