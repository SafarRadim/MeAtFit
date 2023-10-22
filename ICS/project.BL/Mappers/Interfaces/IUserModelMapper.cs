using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public interface IUserModelMapper : IModelMapper<UserEntity, UserListModel, UserDetailModel>
{
}
