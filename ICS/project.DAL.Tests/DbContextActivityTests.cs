using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;
using Xunit;
using Xunit.Abstractions;
using project.Common.Tests;
using project.Common.Tests.Seeds;

namespace project.DAL.Tests;

public class DbContextActivityTests : DbContextTestsBase
{
    public DbContextActivityTests(ITestOutputHelper output) : base(output)
    {
    }

    [Fact]
    public async Task AddNew_Activity_Persisted()
    {
        ActivityEntity entity = new()
        {
            Id = Guid.Parse("475c6384-c34f-4ea0-a192-96f648d85b6a"),
            Name = "A",
            Description = "A",
            Start = new DateTime(2023, 1, 1, 0, 0, 0),
            End = new DateTime(2023, 1, 5, 1, 0, 0),
            UserEntityId = Guid.Parse("d8f5eb07-5d42-4e2f-bbf3-fc6824e6c2ed")
        };

        TrackerDbContextSUT.Activities.Add(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory
            .CreateDbContextAsync();
        var actualEntities = await dbx.Activities
            .SingleAsync(i => i.Id == entity.Id);

        DeepAssert.Equal(entity, actualEntities);
    }

    [Fact]
    public async Task AddNew_ActivityWithTagActivities_Persisted()
    {
        var entity = ActivitySeeds.ActivityEmptyEntity with
        {
            Name = "test",
            Description = "test",
            Start = new DateTime(2023, 1, 1, 0, 0, 0),
            End = new DateTime(2023, 1, 5, 1, 0, 0),
            UserEntityId = Guid.Parse("d8f5eb07-5d42-4e2f-bbf3-fc6824e6c2ed"),
            TagActivities = new List<TagActivityEntity>
            {
                TagActivitySeeds.TagActivityEmptyEntity with
                {
                    Id = Guid.Parse("bb8b1024-db50-4891-8944-cacc1c40c94c"),
                    TagId = Guid.Parse("7b02f3c4-896e-4ccf-b073-424ac3ab39dc"),
                    Tag = TagSeeds.TagEmptyEntity with
                    {
                        Id = Guid.Parse("7b02f3c4-896e-4ccf-b073-424ac3ab39dc"),
                        Name = "test"
                    },
                    ActivityId = Guid.Parse("6b600045-6c6d-4db0-986c-79f7f4371dfa"),
                    Activity = ActivitySeeds.ActivityEmptyEntity with
                    {
                        Id = Guid.Parse("6b600045-6c6d-4db0-986c-79f7f4371dfa"),
                        Name = "test",
                        Description = "test",
                        Start = new DateTime(2023, 1, 1, 0, 0, 0),
                        End = new DateTime(2023, 1, 5, 1, 0, 0)
                    }
                }
            }
        };

        TrackerDbContextSUT.Activities.Add(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory
            .CreateDbContextAsync();

        var actualEntities = await dbx.Activities
            .Include(i => i.TagActivities!)
            .ThenInclude(i => i.Tag)
            .SingleAsync(i => i.Id == entity.Id);


        DeepAssert.Equal(entity, actualEntities);
    }

    [Fact]
    public async Task GetById_Activities_Development_Retrieved()
    {
        var entity = await TrackerDbContextSUT.Activities.SingleAsync(i => i.Id == ActivitySeeds.Development_Without_TagActivities.Id);

        DeepAssert.Equal(ActivitySeeds.Development_Without_TagActivities, entity);
    }

    
    [Fact]
    public async Task GetAll_Activities_Contains_Development()
    {
        var entities = await TrackerDbContextSUT.Activities
            .ToArrayAsync();

        Assert.Contains(ActivitySeeds.Development_Without_TagActivities, entities);
    }
    
    [Fact]
    public async Task Update_Activity()
    {
        var baseEntity = ActivitySeeds.Development_Without_TagActivities;
        var entity = baseEntity with
        {
            Description = baseEntity.Description + "Updated",
            Start = baseEntity.Start.AddDays(1),
            End = baseEntity.End.AddDays(1)
        };

        TrackerDbContextSUT.Activities.Update(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory
            .CreateDbContextAsync();
        var actualEntity = await dbx.Activities
            .SingleAsync(i => i.Id == entity.Id);

        DeepAssert.Equal(entity, actualEntity);
    }

    [Fact]
    public async Task Delete_TagActivity_With_TagActivities()
    {
        var entity = await TrackerDbContextSUT.TagActivities
            .SingleAsync(i => i.Id == Guid.Parse("a7a31c27-559f-4864-8e7c-6f73ecb267ea"));

        // delete
        TrackerDbContextSUT.TagActivities.Remove(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        Assert.False(await TrackerDbContextSUT.TagActivities
            .AnyAsync(i => i.Id == Guid.Parse("a7a31c27-559f-4864-8e7c-6f73ecb267ea")));
        Assert.True(await TrackerDbContextSUT.Tags
            .AnyAsync(i => i.Id == Guid.Parse("f3975f27-641a-4a2d-aa40-0859f9bd803b")));
        Assert.True(await TrackerDbContextSUT.Activities
            .AnyAsync(i => i.Id == Guid.Parse("55614cc0-39bb-4cf8-adfd-aa83dee2d358")));
    }
}