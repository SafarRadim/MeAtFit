using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Facades;

public interface IActivityFacade : IFacade<ActivityEntity, ActivityListModel, ActivityDetailModel>
{
    Task<IEnumerable<ActivityListModel>> GetActivityListFilteredAsync(Guid id, DateTime? from, DateTime? to);
}