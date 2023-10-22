using System;
using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public interface ITagActivityModelMapper
    : IModelMapper<TagActivityEntity, TagActivityListModel, TagActivityDetailModel>
{
    TagActivityListModel MapToListModel(TagActivityDetailModel detailModel);
    TagActivityEntity MapToEntity(TagActivityDetailModel model, Guid activityId);
    void MapToExistingDetailModel(TagActivityDetailModel existingDetailModel, TagListModel tag);
    TagActivityEntity MapToEntity(TagActivityListModel model, Guid activityId);
}
