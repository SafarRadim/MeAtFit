using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Facades;

public interface IUserFacade : IFacade<UserEntity, UserListModel, UserDetailModel>
{
}
