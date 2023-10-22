using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.DAL.Seeds;

public static class TagActivitySeeds
{
    public static readonly TagActivityEntity CodingDevelopment = new()
    {
        Id = Guid.Parse("1d91c287-fd81-4033-a597-18cfb77da41d"),

        TagId = TagSeeds.Coding.Id,
        Tag = TagSeeds.Coding,

        ActivityId = ActivitySeeds.Development.Id,
        Activity = ActivitySeeds.Development
    };

    public static readonly TagActivityEntity DebuggingDevelopment = new()    {
        Id = Guid.Parse("a7a31c27-559f-4864-8e7c-6f73ecb267ea"),

        TagId = TagSeeds.Debugging.Id,
        Tag = TagSeeds.Debugging,

        ActivityId = ActivitySeeds.Development.Id,
        Activity = ActivitySeeds.Development
    };

    public static void Seed(this ModelBuilder modelBuilder) =>
        modelBuilder.Entity<TagActivityEntity>().HasData(
            CodingDevelopment with { Tag = null, Activity = null },
            DebuggingDevelopment with { Tag = null, Activity = null }
            );
}


