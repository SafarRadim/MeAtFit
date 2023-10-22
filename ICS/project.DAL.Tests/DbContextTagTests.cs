using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;
using Xunit;
using Xunit.Abstractions;
using project.Common.Tests.Seeds;
using project.Common.Tests;
using Microsoft.EntityFrameworkCore.Internal;

namespace project.DAL.Tests;

public class DbContextTagTests : DbContextTestsBase
{
    public DbContextTagTests(ITestOutputHelper output) : base(output)
    {
    }

    [Fact]
    public async Task AddNew_Tag()
    {
        TagEntity entity = new()
        {
            Id = Guid.Parse("46e5c25f-b85e-4956-9cd3-de79260d02af"),
            Name = "test"
        };

        TrackerDbContextSUT.Tags.Add(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory
            .CreateDbContextAsync();
        var actualEntities = await dbx.Tags
            .SingleAsync(i => i.Id == entity.Id);

        DeepAssert.Equal(entity, actualEntities);
    }

    [Fact]
    public async Task GetById_Tags_Coding_Retrieved()
    {
        var entity = await TrackerDbContextSUT.Tags.SingleAsync(i => i.Id == TagSeeds.Coding.Id);

        DeepAssert.Equal(TagSeeds.Coding, entity);
    }

    [Fact]
    public async Task GetAll_Tags_Contains_Debugging()
    {
        var entities = await TrackerDbContextSUT.Tags
            .ToArrayAsync();
    
        Assert.Contains(TagSeeds.Debugging, entities);
    }

    [Fact]
    public async Task Update_Debugging_Persisted()
    {
        var baseEntity = TagSeeds.Debugging;
        var entity = baseEntity with
        {
            Name = baseEntity.Name + "Updated"
        };

        TrackerDbContextSUT.Tags.Update(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var actualEntity = await dbx.Tags.SingleAsync(i => i.Id == entity.Id);
        DeepAssert.Equal(entity, actualEntity);
    }

    [Fact]
    public async Task Delete_Coding()
    {
        var baseEntity = TagSeeds.Coding;

        TrackerDbContextSUT.Tags.Remove(baseEntity);
        await TrackerDbContextSUT.SaveChangesAsync();

        Assert.False(await TrackerDbContextSUT.Tags.AnyAsync(i => i.Id == baseEntity.Id));
    }
}