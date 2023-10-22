using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.Common.Tests.Seeds;

public static class UserProjectSeeds
{
    public static readonly UserProjectEntity UserProjectEmptyEntity = new() {
        Id = default,
        UserId = default,
        User = default,
        ProjectId = default,
        Project = default
    };

    public static readonly UserProjectEntity JozefTrackerSoftware = new()
    {
        Id = Guid.Parse("ece70dcf-ff14-4f03-948d-044e3ef57462"),
        UserId = UserSeeds.Jozef.Id,
        User = UserSeeds.Jozef,

        ProjectId = ProjectSeeds.TrackerSoftware.Id,
        Project = ProjectSeeds.TrackerSoftware
    };
    public static void Seed(this ModelBuilder modelBuilder) =>
        modelBuilder.Entity<UserProjectEntity>().HasData(
            JozefTrackerSoftware with { User = null, Project = null }
        );
}


