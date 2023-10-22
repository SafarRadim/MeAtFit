using System.Reflection.Metadata;
using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;
using Xunit;
using Xunit.Abstractions;
using project.Common.Tests.Seeds;
using project.Common.Tests;
using Microsoft.EntityFrameworkCore.Internal;
using Microsoft.Identity.Client;

namespace project.DAL.Tests;

public class DbContextUserProjectTests : DbContextTestsBase
{
    public DbContextUserProjectTests(ITestOutputHelper output) : base(output)
    {
    }

    [Fact]
    public async Task AddNew_UserProject()
    {
        var entity = UserProjectSeeds.UserProjectEmptyEntity with
        {
            Id = Guid.Parse("c2cd26cc-cc43-4368-8dcd-7fbaf0668446"),
            UserId = Guid.Parse("bc6b5bb8-342b-4f30-be8b-66e785930574"),
            User = UserSeeds.UserEmptyEntity with
            {
                Id = Guid.Parse("bc6b5bb8-342b-4f30-be8b-66e785930574"),
                Name = "User1",
                Surname = "User1",
                Email = "caw@gmail.com"
            },
            ProjectId = Guid.Parse("d5b31175-af4f-462e-9025-d3a788abd8ec"),
            Project = ProjectSeeds.ProjectEmptyEntity with
            {
                Id = Guid.Parse("d5b31175-af4f-462e-9025-d3a788abd8ec"),
                Name = "Project1",
                Description = "Project1"
            }
        };

        TrackerDbContextSUT.UserProjects.Add(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory
            .CreateDbContextAsync();
        var actualEntities = await dbx.UserProjects
            .Include(i => i.User)
            .Include(i => i.Project)
            .SingleAsync(i => i.Id == entity.Id);

       DeepAssert.Equal(entity, actualEntities);
    }

    [Fact]
    public async Task GetById_UserProject_JozefTrackerSoftware()
    {
        var entity = await TrackerDbContextSUT.UserProjects
            .SingleAsync(i => i.Id == Guid.Parse("ece70dcf-ff14-4f03-948d-044e3ef57462"));

        DeepAssert.Equal(UserProjectSeeds.JozefTrackerSoftware.Id, entity.Id);
    }

    [Fact]
    public async Task Delete_UserProject_With_UserProjects()
    {
        var entity = await TrackerDbContextSUT.UserProjects
            .SingleAsync(i => i.Id == Guid.Parse("ece70dcf-ff14-4f03-948d-044e3ef57462"));

        TrackerDbContextSUT.UserProjects.Remove(entity);
        await TrackerDbContextSUT.SaveChangesAsync();

        Assert.False(await TrackerDbContextSUT.UserProjects
                   .AnyAsync(i => i.Id == Guid.Parse("ece70dcf-ff14-4f03-948d-044e3ef57462")));
    }
}