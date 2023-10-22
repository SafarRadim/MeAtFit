using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Facades;

public interface IProjectFacade : IFacade<ProjectEntity, ProjectListModel, ProjectDetailModel>
{
}