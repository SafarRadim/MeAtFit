using System;
using System.Threading.Tasks;
using project.BL.Models;

namespace project.BL.Facades;

public interface ITagActivityFacade
{
    Task<TagActivityDetailModel> SaveAsync(TagActivityDetailModel model, Guid UserId);
    Task SaveAsync(TagActivityListModel model, Guid UserId);
    Task DeleteAsync(Guid id);
}