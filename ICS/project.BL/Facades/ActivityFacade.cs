using Microsoft.EntityFrameworkCore;
using project.BL.Mappers;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;
using project.DAL.UnitOfWork;
namespace project.BL.Facades;

public class ActivityFacade : FacadeBase<ActivityEntity, ActivityListModel, ActivityDetailModel, ActivityEntityMapper>,
    IActivityFacade
{
    public ActivityFacade(
        IUnitOfWorkFactory unitOfWorkFactory,
        IActivityModelMapper modelMapper)
        : base(unitOfWorkFactory, modelMapper)
    {
    }

    public virtual async Task<IEnumerable<ActivityListModel>> GetActivityListFilteredAsync(Guid userId, DateTime? from, DateTime? to)
    {
        if (from is null) from = DateTime.MinValue;
        if (to is null) to = DateTime.MaxValue;
        
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IQueryable<ActivityEntity> query = uow
        .GetRepository<ActivityEntity, ActivityEntityMapper>()
        .Get()
        .Where(activity => activity.UserEntityId == userId)
        .Where(activity => activity.Start >= from && activity.Start <= to)
        ;

        List<ActivityEntity> activities = await query.ToListAsync();

        IEnumerable<ActivityListModel> activityList = ModelMapper.MapToListModel(activities);

        return activityList;
    }

    public override async Task<ActivityDetailModel> SaveAsync(ActivityDetailModel model)
    {
        ActivityDetailModel result;

        GuardCollectionsAreNotSet(model);

        ActivityEntity entity = ModelMapper.MapToEntity(model);

        IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<ActivityEntity> repository = uow.GetRepository<ActivityEntity, ActivityEntityMapper>();

        bool overlaps = await repository.Get()
        .AnyAsync(existingActivity =>
            existingActivity.Id != entity.Id &&
            existingActivity.UserEntityId == entity.UserEntityId &&
            existingActivity.Start <= entity.End &&
            existingActivity.End >= entity.Start
        );

        if (overlaps)
        {
            throw new InvalidOperationException("The new activity overlaps with existing activities.");
        }

        if (await repository.ExistsAsync(entity))
        {
            ActivityEntity updatedEntity = await repository.UpdateAsync(entity);
            result = ModelMapper.MapToDetailModel(updatedEntity);
        }
        else
        {
            entity.Id = Guid.NewGuid();
            ActivityEntity insertedEntity = await repository.InsertAsync(entity);
            result = ModelMapper.MapToDetailModel(insertedEntity);
        }

        await uow.CommitAsync();

        return result;
    }

    protected override string IncludesNavigationPathDetail =>
        $"{nameof(ActivityEntity.TagActivities)}.{nameof(TagActivityEntity.Tag)}";
}
