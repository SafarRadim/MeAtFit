using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;
using Xunit;
using Xunit.Abstractions;
using project.Common.Tests;
using project.Common.Tests.Seeds;

namespace project.DAL.Tests;

public class DbContextTagActivityTests : DbContextTestsBase
{
    public DbContextTagActivityTests(ITestOutputHelper output) : base(output)
    {
    }

    [Fact]
    public async Task AddNew_TagActivity_With_Tag_Activity()
    {
        var entity = TagActivitySeeds.TagActivityEmptyEntity with
        {
            Id = Guid.Parse("28968f83-dc86-4ec6-b5fd-ac712233107d"),
            TagId = Guid.Parse("98c75d44-b49b-4c17-af12-bef79de7a5e9"),
            Tag = TagSeeds.TagEmptyEntity with
            {
                Id = Guid.Parse("98c75d44-b49b-4c17-af12-bef79de7a5e9"),
                Name = "Tag1"
            },
            ActivityId = Guid.Parse("c0a8b5b1-0b9b-4b9f-9c1b-1b5b1b9b4b9f"),
            Activity = ActivitySeeds.ActivityEmptyEntity with
            {
                Id = Guid.Parse("c0a8b5b1-0b9b-4b9f-9c1b-1b5b1b9b4b9f"),
                Description = "Activity1"
            }
        };

        TrackerDbContextSUT.TagActivities.Add(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory
            .CreateDbContextAsync();
        var actualEntities = await dbx.TagActivities
            .Include(i => i.Tag)
            .Include(i => i.Activity)
            .SingleAsync(i => i.Id == entity.Id);

        DeepAssert.Equal(entity, actualEntities);
    }

    [Fact]
    public async Task GetById_TagActivity_DebuggingDevelopment()
    {
        var entity = await TrackerDbContextSUT.TagActivities
            .Include(i => i.Tag)
            .Include(i => i.Activity)
            .Include(i => i.Activity!.TagActivities)
            .SingleAsync(i => i.Id == Guid.Parse("a7a31c27-559f-4864-8e7c-6f73ecb267ea"));


        DeepAssert.Equal(TagActivitySeeds.DebuggingDevelopment.Tag, entity.Tag);

        DeepAssert.Equal(TagActivitySeeds.DebuggingDevelopment.Id, entity.Id);
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