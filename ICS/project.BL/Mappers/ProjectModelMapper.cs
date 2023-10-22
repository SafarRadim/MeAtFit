using project.BL.Models;
using project.DAL.Entities;
using project.BL.Mappers;

namespace project.BL.Mappers;

public class ProjectModelMapper : ModelMapperBase<ProjectEntity, ProjectListModel, ProjectDetailModel>, IProjectModelMapper
{
    private readonly IUserProjectModelMapper _userProjectModelMapper;

    public ProjectModelMapper(IUserProjectModelMapper userProjectModelMapper) =>
        _userProjectModelMapper = userProjectModelMapper;

    public override ProjectListModel MapToListModel(ProjectEntity? entity)
        => entity is null
            ? ProjectListModel.Empty
            : new ProjectListModel
            {
                Id = entity.Id,
                Name = entity.Name
            };

    public override ProjectDetailModel MapToDetailModel(ProjectEntity? entity)
        => entity is null
            ? ProjectDetailModel.Empty
            : new ProjectDetailModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Description = entity.Description,
                Activities = null,
                Users = (entity.UserProjects == null) ? Enumerable.Empty<UserProjectListModel>().ToObservableCollection() : (_userProjectModelMapper?.MapToListModel(entity.UserProjects) ?? Enumerable.Empty<UserProjectListModel>()).ToObservableCollection()
            };

    public override ProjectEntity MapToEntity(ProjectDetailModel model)
        => new()
        {
            Id = model.Id,
            Name = model.Name,
            Description = model.Description
        };
}
