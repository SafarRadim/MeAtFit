using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.Common.Tests.Seeds;

public static class ActivitySeeds
{
    public static readonly ActivityEntity ActivityEmptyEntity = new()
    {
        Id = Guid.NewGuid(),
        Description = string.Empty,
        Start = DateTime.MinValue,
        End = DateTime.MinValue,
        Name = string.Empty
    };

    public static readonly ActivityEntity Development = new()
    {
        Id = Guid.Parse("55614cc0-39bb-4cf8-adfd-aa83dee2d358"),

        Start = new DateTime(2023, 4, 2, 12, 30, 0),
        End = new DateTime(2023, 4, 2, 13, 00, 0),

        Description = "Fixing frontend data fetch error from paywall API",
        Name = "Fix FE"
    };

    public static readonly ActivityEntity Development_Without_TagActivities = new()
    {
        Id = Guid.Parse("79d8443a-0c4e-4c21-939f-b843b006f94d"),

        Start = new DateTime(2023, 4, 2, 12, 30, 0),
        End = new DateTime(2023, 4, 2, 13, 00, 0),

        Description = "Fixing fix DB queries that bottleneck out app performance",
        Name = "Fix BE quer"
    };

    static ActivitySeeds()
    {
        Development.TagActivities ??= new List<TagActivityEntity>(); // to prevent null reference exception
        Development.TagActivities.Add(TagActivitySeeds.DebuggingDevelopment);
        Development.TagActivities.Add(TagActivitySeeds.CodingDevelopment);
    }

    public static void Seed(this ModelBuilder modelBuilder) =>
        modelBuilder.Entity<ActivityEntity>().HasData(
            Development with { TagActivities = Array.Empty<TagActivityEntity>() },
            Development_Without_TagActivities with { TagActivities = Array.Empty<TagActivityEntity>() }
        );
}