using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public interface IActivityModelMapper : IModelMapper<ActivityEntity, ActivityListModel, ActivityDetailModel>
{
}
