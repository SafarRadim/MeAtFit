using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.Common.Tests.Seeds;

public static class ProjectSeeds
{
    public static readonly ProjectEntity ProjectEmptyEntity = new()
    {
        Id = default,
        Name = default!,
        Activities = default,
        UserProjects = default,
        Description = default!
    };

    public static readonly ProjectEntity TrackerSoftware = new()
    {
        Id = Guid.Parse("8112538d-bc61-48d7-a3fd-b5b251e77d8c"),

        Name = "Tracker Software",
        Description = "Developing tracking software for managing team acitivities"
    };

    public static readonly ProjectEntity newGenerationTracker = new()
    {
        Id = Guid.Parse("81121111-bc61-48d7-a3fd-b5b251e77d8c"),

        Name = "NG Software",
        
        Description = "New software project for that will make us rich"
    };

    static ProjectSeeds()
    {
        TrackerSoftware.Activities ??= new List<ActivityEntity>(); // to prevent null reference exception
        TrackerSoftware.Activities.Add(ActivitySeeds.Development);
    }

    public static void Seed(this ModelBuilder modelBuilder) =>
        modelBuilder.Entity<ProjectEntity>().HasData(
            TrackerSoftware with { UserProjects = Array.Empty<UserProjectEntity>(), Activities = Array.Empty<ActivityEntity>() },
            newGenerationTracker with { UserProjects = Array.Empty<UserProjectEntity>(), Activities = Array.Empty<ActivityEntity>() }
        );
}