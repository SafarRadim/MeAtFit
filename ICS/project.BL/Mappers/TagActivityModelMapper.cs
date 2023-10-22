using System;
using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public class TagActivityModelMapper :
    ModelMapperBase<TagActivityEntity, TagActivityListModel, TagActivityDetailModel>,
    ITagActivityModelMapper
{
    public override TagActivityListModel MapToListModel(TagActivityEntity? entity)
        => entity?.Tag is null
            ? TagActivityListModel.Empty
            : new TagActivityListModel
            {
                Id = entity.Id,
                TagId = entity.Tag.Id,
                TagName = entity.Tag.Name
            };

    public override TagActivityDetailModel MapToDetailModel(TagActivityEntity? entity)
        => entity?.Tag is null
            ? TagActivityDetailModel.Empty
            : new TagActivityDetailModel
            {
                Id = entity.Id,
                TagId = entity.Tag.Id,
                TagName = entity.Tag.Name
            };

    public TagActivityListModel MapToListModel(TagActivityDetailModel detailModel)
        => new()
        {
            Id = detailModel.Id,
            TagId = detailModel.TagId,
            TagName = detailModel.TagName
        };

    public void MapToExistingDetailModel(TagActivityDetailModel existingDetailModel,
        TagListModel tag)
    {
        existingDetailModel.TagId = tag.Id;
        existingDetailModel.TagName = tag.TagName;
    }

    public override TagActivityEntity MapToEntity(TagActivityDetailModel model)
        => throw new NotImplementedException("This method is unsupported. Use the other overload.");


    public TagActivityEntity MapToEntity(TagActivityDetailModel model, Guid activityId)
        => new()
        {
            Id = model.Id,
            ActivityId = activityId,
            TagId = model.TagId
        };

    public TagActivityEntity MapToEntity(TagActivityListModel model, Guid activityId)
        => new()
        {
            Id = model.Id,
            ActivityId = activityId,
            TagId = model.TagId
        };
}
