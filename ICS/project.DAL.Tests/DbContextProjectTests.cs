using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;
using Xunit;
using Xunit.Abstractions;
using project.Common.Tests.Seeds;
using project.Common.Tests;


namespace project.DAL.Tests;

public class DbContextProjectTests : DbContextTestsBase
{
    public DbContextProjectTests(ITestOutputHelper output) : base(output)
    {
    }

    [Fact]
    public async Task AddNew_Project()
    {
        ProjectEntity entity = new()
        {
            Id = Guid.NewGuid(),
            Name = "A",
            Description = "A"
        };

        TrackerDbContextSUT.Projects.Add(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory
            .CreateDbContextAsync();
        var actualEntities = await dbx.Projects
            .SingleAsync(i => i.Id == entity.Id);

        DeepAssert.Equal(entity, actualEntities);
    }

    [Fact]
    public async Task AddNew_User_With_Activities()
    {
        var entity = ProjectSeeds.ProjectEmptyEntity with
        {
            Id = Guid.Parse("7274f2c8-a21a-4358-abad-13a361a0c687"),
            Name = "A",
            Description = "A",

            Activities = new List<ActivityEntity>
            {
                ActivitySeeds.ActivityEmptyEntity with
                {
                    Id = Guid.Parse("b0ae69a6-3fe8-4152-8b9f-f696bd865431"),
                    Description = "testing"
                }
            }
        };

        TrackerDbContextSUT.Projects.Add(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory
            .CreateDbContextAsync();

        var actualEntities = await dbx.Projects
            .Include(i => i.Activities)
            .SingleAsync(i => i.Id == entity.Id);

        DeepAssert.Equal(entity, actualEntities);
    }

    [Fact]
    public async Task GetById_Projects_TrackerSoftware_Retrieved()
    {
        var entity = await TrackerDbContextSUT.Projects
            .SingleAsync(i => i.Id == ProjectSeeds.TrackerSoftware.Id);

        Assert.Equal(ProjectSeeds.TrackerSoftware.Id, entity.Id);
    }
    [Fact]
    public async Task GetAll_Projects_Contains_TrackerSoftware()
    {
        var entities = await TrackerDbContextSUT.Projects
            .ToListAsync();

        Assert.Contains(ProjectSeeds.newGenerationTracker, entities);
    }

    [Fact]
    public async Task Delete_TrackerSoftware()
    {
        var baseEntity = ProjectSeeds.TrackerSoftware;

        TrackerDbContextSUT.Projects.Remove(baseEntity);
        await TrackerDbContextSUT.SaveChangesAsync();

        Assert.False(await TrackerDbContextSUT.Projects.AnyAsync(i => i.Id == baseEntity.Id));
    }
}