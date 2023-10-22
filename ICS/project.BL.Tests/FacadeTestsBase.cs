using project.BL;
using project.Common;
using project.Common.Tests.Factories;
using project.DAL;
using project.DAL.Mappers;
using project.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;
using project.BL.Mappers;
using project.BL.Models;
using project.Common.Tests;
using Xunit;
using Xunit.Abstractions;

namespace project.BL.Tests;

public class FacadeTestsBase : IAsyncLifetime
{
    protected FacadeTestsBase(ITestOutputHelper output)
    {
        XUnitTestOutputConverter converter = new(output);
        Console.SetOut(converter);

        DbContextFactory = new DbContextSqLiteTestingFactory(GetType().FullName!, seedTestingData: true);

        //EntityMappers
        TagEntityMapper = new TagEntityMapper();
        ActivityEntityMapper = new ActivityEntityMapper();
        TagActivityEntityMapper = new TagActivityEntityMapper();
        UserEntityMapper = new UserEntityMapper();
        ProjectEntityMapper = new ProjectEntityMapper();
        UserProjectEntityMapper = new UserProjectEntityMapper();

        //ModelMappers
        TagModelMapper = new TagModelMapper();
        TagActivityModelMapper = new TagActivityModelMapper();
        ActivityModelMapper = new ActivityModelMapper(TagActivityModelMapper);
        UserModelMapper = new UserModelMapper(ActivityModelMapper);
        UserProjectModelMapper = new UserProjectModelMapper();
        ProjectModelMapper = new ProjectModelMapper(UserProjectModelMapper);

        UnitOfWorkFactory = new UnitOfWorkFactory(DbContextFactory);
    }

    //Mappers
    protected IDbContextFactory<TrackerDbContext> DbContextFactory { get; }

    protected TagEntityMapper TagEntityMapper { get; }
    protected ActivityEntityMapper ActivityEntityMapper { get; }
    protected TagActivityEntityMapper TagActivityEntityMapper { get; }
    protected UserEntityMapper UserEntityMapper { get; }
    protected ProjectEntityMapper ProjectEntityMapper { get; }
    protected UserProjectEntityMapper UserProjectEntityMapper { get; }

    //Models
    protected IActivityModelMapper ActivityModelMapper { get; }
    protected ITagModelMapper TagModelMapper { get; }
    protected ITagActivityModelMapper TagActivityModelMapper { get; }
    protected IUserModelMapper UserModelMapper { get; }
    protected IProjectModelMapper ProjectModelMapper { get; }
    protected IUserProjectModelMapper UserProjectModelMapper { get; }

    protected UnitOfWorkFactory UnitOfWorkFactory { get; }

    public async Task InitializeAsync()
    {
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        await dbx.Database.EnsureDeletedAsync();
        await dbx.Database.EnsureCreatedAsync();
    }

    public async Task DisposeAsync()
    {
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        await dbx.Database.EnsureDeletedAsync();
    }
}