using System;
using System.Threading.Tasks;
using project.BL.Mappers;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;
using project.DAL.UnitOfWork;
namespace project.BL.Facades;
public class UserProjectFacade :
    FacadeBase<UserProjectEntity, UserProjectListModel, UserProjectDetailModel,
        UserProjectEntityMapper>, IUserProjectFacade
{
    private readonly IUserProjectModelMapper _UserProjectModelMapper;

    public UserProjectFacade(
        IUnitOfWorkFactory unitOfWorkFactory,
        IUserProjectModelMapper UserProjectModelMapper)
        : base(unitOfWorkFactory, UserProjectModelMapper) =>
        _UserProjectModelMapper = UserProjectModelMapper;

    public async Task SaveAsync(UserProjectListModel model, Guid UserId)
    {
        UserProjectEntity entity = _UserProjectModelMapper.MapToEntity(model, UserId);

        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<UserProjectEntity> repository =
            uow.GetRepository<UserProjectEntity, UserProjectEntityMapper>();

        if (await repository.ExistsAsync(entity))
        {
            await repository.UpdateAsync(entity);
            await uow.CommitAsync();
        }
    }

    public async Task<UserProjectDetailModel> SaveAsync(UserProjectDetailModel model, Guid UserId)
    {
        UserProjectEntity entity = _UserProjectModelMapper.MapToEntity(model, UserId);

        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<UserProjectEntity> repository =
            uow.GetRepository<UserProjectEntity, UserProjectEntityMapper>();

        await repository.InsertAsync(entity);
        await uow.CommitAsync();

        return ModelMapper.MapToDetailModel(entity);
    }
}
