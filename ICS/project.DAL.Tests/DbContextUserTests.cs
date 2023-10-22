using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;
using Xunit;
using Xunit.Abstractions;
using project.Common.Tests;
using project.Common.Tests.Seeds;

namespace project.DAL.Tests;

public class DbContextUserTests : DbContextTestsBase
{
    public DbContextUserTests(ITestOutputHelper output) : base(output)
    {
    }

    [Fact]
    public async Task AddNew_User()
    {
        UserEntity entity = new()
        {
            Id = Guid.Parse("0358bf25-a823-4afa-8164-5b9d5bbab634"),
            Name = "Lapi",
            Surname = "Cuki",
            Email = "najs@gmail.com"
        };

        TrackerDbContextSUT.Users.Add(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory
            .CreateDbContextAsync();
        var actualEntities = await dbx.Users
            .SingleAsync(i => i.Id == entity.Id);

        DeepAssert.Equal(entity, actualEntities);
    }

    [Fact]
    public async Task AddNew_User_With_Activities()
    {
        var entity = UserSeeds.UserEmptyEntity with
        {
            Id = Guid.Parse("0358bf25-a823-4afa-8164-5b9d5bbab634"),
            Name = "testing",
            Surname = "user",
            Email = "toto@gmail.com",
            Activities = new List<ActivityEntity>
            {
                ActivitySeeds.ActivityEmptyEntity with
                {
                    Id = Guid.Parse("b0ae69a6-3fe8-4152-8b9f-f696bd865431"),
                    Description = "testing"
                }
            },
            UserProjects = Array.Empty<UserProjectEntity>()
        };

        TrackerDbContextSUT.Users.Add(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var actualEntity = await dbx.Users
            .Include(i => i.Activities)
            .SingleAsync(i => i.Id == entity.Id);

        DeepAssert.Equal(entity, actualEntity);
    }

    [Fact]
    public async Task GetById_IncludingActivities_User()
    {
        var entity = await TrackerDbContextSUT.Users
            .Include(i => i.Activities)
            
            .SingleAsync(i => i.Id == UserSeeds.Jozef.Id);

        Assert.Equal(UserSeeds.Jozef.Id, entity.Id);
    }

    [Fact(Skip = "commented out")]
    public async Task Update_User()
    {
        var baseEntity = UserSeeds.Jozef;

        var entity = baseEntity with
        {
            Name = baseEntity.Name + "testing",
            Surname = baseEntity.Surname + "testing",
            Email = baseEntity.Email + "testing"
        };

        TrackerDbContextSUT.Update(entity);

        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var actualEntity = await dbx.Users
            .Include(i => i.Activities)
            .SingleAsync(i => i.Id == entity.Id);

        DeepAssert.Equal(entity, actualEntity);
    }

    [Fact]
    public async Task DeleteById_JozefWithTagActivities()
    {
        var baseEntity = UserSeeds.Jozef;

        TrackerDbContextSUT.Users.Remove(
            TrackerDbContextSUT.Users.Single(i => i.Id == baseEntity.Id));

        await TrackerDbContextSUT.SaveChangesAsync();

        Assert.False(await TrackerDbContextSUT.Users.AnyAsync(i => i.Id == baseEntity.Id));

        Assert.True(await TrackerDbContextSUT.Activities.AnyAsync(i => i.Id == ActivitySeeds.Development.Id));
    }
}


