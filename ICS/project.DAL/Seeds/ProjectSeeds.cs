using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.DAL.Seeds;

public static class ProjectSeeds
{
    public static readonly ProjectEntity TrackerSoftware = new()
    {
        Id = Guid.Parse("8112538d-bc61-48d7-a3fd-b5b251e77d8c"),

        Name = "Tracker Software",

        Description = "Software used for tracking activities"
    };

    static ProjectSeeds()
    {
        TrackerSoftware.Activities ??= new List<ActivityEntity>(); // to prevent null reference exception
        TrackerSoftware.Activities.Add(ActivitySeeds.Development);
        
        TrackerSoftware.UserProjects ??= new List<UserProjectEntity>();
        TrackerSoftware.UserProjects.Add(UserProjectSeeds.JozefTrackerSoftware);

    }

    public static void Seed(this ModelBuilder modelBuilder) =>
        modelBuilder.Entity<ProjectEntity>().HasData(
            TrackerSoftware with { UserProjects = Array.Empty<UserProjectEntity>(), Activities = Array.Empty<ActivityEntity>() }
        );
}