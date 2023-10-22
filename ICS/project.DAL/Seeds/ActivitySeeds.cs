using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.DAL.Seeds;

public static class ActivitySeeds
{
    public static readonly ActivityEntity Development = new()
    {
        Id = Guid.Parse("55614cc0-39bb-4cf8-adfd-aa83dee2d358"),

        Start = new DateTime(2023, 4, 2, 12, 30, 0),
        End = new DateTime(2023, 4, 2, 13, 00, 0),
        UserEntityId = UserSeeds.Jozef.Id,
        Name = "Coding",
        Description = "Coding"
    };

    static ActivitySeeds()
    {
       Development.TagActivities ??= new List<TagActivityEntity>(); // to prevent null reference exception
       Development.TagActivities.Add(TagActivitySeeds.DebuggingDevelopment);
       Development.TagActivities.Add(TagActivitySeeds.CodingDevelopment);
    }

    public static void Seed(this ModelBuilder modelBuilder) =>
        modelBuilder.Entity<ActivityEntity>().HasData(
            Development with { TagActivities = Array.Empty<TagActivityEntity>() }
        );
}