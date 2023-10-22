using System;
using project.BL.Models;
using project.DAL.Entities;
using project.BL.Mappers;

namespace project.BL.Mappers;

public interface IUserProjectModelMapper
    : IModelMapper<UserProjectEntity, UserProjectListModel, UserProjectDetailModel>
{
    UserProjectListModel MapToListModel(UserProjectDetailModel detailModel);
    UserProjectEntity MapToEntity(UserProjectDetailModel model, Guid userId);
    void MapToExistingDetailModel(UserProjectDetailModel existingDetailModel, UserListModel user);
    UserProjectEntity MapToEntity(UserProjectListModel model, Guid userId);
}
