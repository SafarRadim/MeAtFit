using Microsoft.EntityFrameworkCore;

namespace project.DAL.UnitOfWork;

public class UnitOfWorkFactory : IUnitOfWorkFactory
{
    private readonly IDbContextFactory<TrackerDbContext> _dbContextFactory;

    public UnitOfWorkFactory(IDbContextFactory<TrackerDbContext> dbContextFactory) =>
        _dbContextFactory = dbContextFactory;

    public IUnitOfWork Create() => new UnitOfWork(_dbContextFactory.CreateDbContext());
}
