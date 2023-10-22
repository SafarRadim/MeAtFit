using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public interface IProjectModelMapper : IModelMapper<ProjectEntity, ProjectListModel, ProjectDetailModel>
{
}
