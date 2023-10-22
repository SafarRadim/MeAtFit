using project.BL.Models;
using project.DAL.Entities;
using project.BL.Mappers;
using System.Text;

namespace project.BL.Mappers;

public class ActivityModelMapper : ModelMapperBase<ActivityEntity, ActivityListModel, ActivityDetailModel>, IActivityModelMapper
{
    private readonly ITagActivityModelMapper _tagActivityModelMapper;

    public ActivityModelMapper(ITagActivityModelMapper TagActivityModelMapper) =>
        _tagActivityModelMapper = TagActivityModelMapper;

    public override ActivityListModel MapToListModel(ActivityEntity? entity)
        => entity is null
            ? ActivityListModel.Empty
            : new ActivityListModel
            {
                Id = entity.Id,
                Name = entity.Name,
                End = entity.End
            };

    public override ActivityDetailModel MapToDetailModel(ActivityEntity? entity)
        => entity is null
            ? ActivityDetailModel.Empty
            : new ActivityDetailModel
            {
                Id = entity.Id,
                Description = entity.Description,
                Name = entity.Name,
                Start = entity.Start,
                End = entity.End,
                ProjectId = entity.ProjectId,
                TagActivities = entity.TagActivities is null ? Enumerable.Empty<TagActivityListModel>().ToObservableCollection() : _tagActivityModelMapper.MapToListModel(entity.TagActivities)
                    .ToObservableCollection()
            };

    public override ActivityEntity MapToEntity(ActivityDetailModel model)
        => new()
        {
            Id = model.Id,
            ProjectId = (Guid)(model.ProjectId ?? Guid.Empty),
            Description = model.Description,
            Name = model.Name,
            Start = model.Start,
            End = model.End,
            UserEntityId = model.UserId
        };
}
