using project.BL.Mappers;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;
using project.DAL.UnitOfWork;
namespace project.BL.Facades;
public class TagActivityFacade :
    FacadeBase<TagActivityEntity, TagActivityListModel, TagActivityDetailModel,
        TagActivityEntityMapper>, ITagActivityFacade
{
    private readonly ITagActivityModelMapper _tagActivityModelMapper;

    public TagActivityFacade(
        IUnitOfWorkFactory unitOfWorkFactory,
        ITagActivityModelMapper TagActivityModelMapper)
        : base(unitOfWorkFactory, TagActivityModelMapper) =>
        _tagActivityModelMapper = TagActivityModelMapper;

    public async Task SaveAsync(TagActivityListModel model, Guid activityId)
    {
        TagActivityEntity entity = _tagActivityModelMapper.MapToEntity(model, activityId);

        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<TagActivityEntity> repository =
            uow.GetRepository<TagActivityEntity, TagActivityEntityMapper>();

        if (await repository.ExistsAsync(entity))
        {
            await repository.UpdateAsync(entity);
            await uow.CommitAsync();
        }
    }

    public async Task<TagActivityDetailModel> SaveAsync(TagActivityDetailModel model, Guid UserId)
    {
        TagActivityEntity entity = _tagActivityModelMapper.MapToEntity(model, UserId);

        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<TagActivityEntity> repository =
            uow.GetRepository<TagActivityEntity, TagActivityEntityMapper>();

        await repository.InsertAsync(entity);
        await uow.CommitAsync();

        return ModelMapper.MapToDetailModel(entity);
    }
}
