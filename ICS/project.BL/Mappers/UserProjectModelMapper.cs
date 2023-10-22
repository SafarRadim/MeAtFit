using System;
using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public class UserProjectModelMapper :
    ModelMapperBase<UserProjectEntity, UserProjectListModel, UserProjectDetailModel>,
    IUserProjectModelMapper
{
    public override UserProjectListModel MapToListModel(UserProjectEntity? entity)
        => entity?.User is null
            ? UserProjectListModel.Empty
            : new UserProjectListModel
            {
                Id = entity.Id,
                UserId = entity.User.Id,
                UserName = entity.User.Name
            };

    public override UserProjectDetailModel MapToDetailModel(UserProjectEntity? entity)
        => entity?.User is null
            ? UserProjectDetailModel.Empty
            : new UserProjectDetailModel
            {
                Id = entity.Id,
                UserId = entity.User.Id,
                UserName = entity.User.Name
            };

    public UserProjectListModel MapToListModel(UserProjectDetailModel detailModel)
        => new()
        {
            Id = detailModel.Id,
            UserId = detailModel.UserId,
            UserName = detailModel.UserName
        };

    public void MapToExistingDetailModel(UserProjectDetailModel existingDetailModel,
        UserListModel user)
    {
        existingDetailModel.UserId = user.Id;
        existingDetailModel.UserName = user.Name;
    }

    public override UserProjectEntity MapToEntity(UserProjectDetailModel model)
        => throw new NotImplementedException("This method is unsupported. Use the other overload.");


    public UserProjectEntity MapToEntity(UserProjectDetailModel model, Guid projectId)
        => new()
        {
            Id = model.Id,
            UserId = model.UserId,
            ProjectId = projectId,
        };

    public UserProjectEntity MapToEntity(UserProjectListModel model, Guid projectId)
        => new()
        {
            Id = model.Id,
            UserId = model.UserId,
            ProjectId = projectId,
        };
}
