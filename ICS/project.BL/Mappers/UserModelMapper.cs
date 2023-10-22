using project.BL.Models;
using project.DAL.Entities;
using project.BL.Mappers;

namespace project.BL.Mappers;

public class UserModelMapper : ModelMapperBase<UserEntity, UserListModel, UserDetailModel>, IUserModelMapper
{
    private readonly IActivityModelMapper _activityModelMapper;


    public UserModelMapper(IActivityModelMapper activityModelMapper) =>
        _activityModelMapper = activityModelMapper;

    public override UserListModel MapToListModel(UserEntity? entity)
        => entity is null
            ? UserListModel.Empty
            : new UserListModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Surname = entity.Surname,
                ImageUrl = entity.ImageUrl
            };

    public override UserDetailModel MapToDetailModel(UserEntity? entity)
        => entity is null
            ? UserDetailModel.Empty
            : new UserDetailModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Surname = entity.Surname,
                Email = entity.Email,
                ImageUrl = entity.ImageUrl,
                Activities = (entity.Activities == null) ? Enumerable.Empty<ActivityListModel>().ToObservableCollection() : (_activityModelMapper?.MapToListModel(entity.Activities) ?? Enumerable.Empty<ActivityListModel>()).ToObservableCollection()
            };

    public override UserEntity MapToEntity(UserDetailModel model)
        => new()
        {
            Id = model.Id,
            Name = model.Name,
            Surname = model.Surname,
            Email = model.Email,
            ImageUrl = model.ImageUrl
        };
}
