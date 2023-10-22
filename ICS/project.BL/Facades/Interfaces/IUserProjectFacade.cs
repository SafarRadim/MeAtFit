using System;
using System.Threading.Tasks;
using project.BL.Models;

namespace project.BL.Facades;

public interface IUserProjectFacade
{
    Task<UserProjectDetailModel> SaveAsync(UserProjectDetailModel model, Guid UserId);
    Task SaveAsync(UserProjectListModel model, Guid UserId);
    Task DeleteAsync(Guid id);
}
