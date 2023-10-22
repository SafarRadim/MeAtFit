using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.Common.Tests.Seeds;

public static class TagSeeds
{
    public static readonly TagEntity TagEmptyEntity = new()
    {
        Id = default,
        Name = default!
    };

    public static readonly TagEntity Coding = new()
    {
        Id = Guid.Parse("b59e3b41-2127-4840-8023-80312a462916"),
        Name = "Coding"
    };

    public static readonly TagEntity Debugging = new()
    {
        Id = Guid.Parse("f3975f27-641a-4a2d-aa40-0859f9bd803b"),
        Name = "Debugging"
    };
    
    public static void Seed(this ModelBuilder modelBuilder) =>
        modelBuilder.Entity<TagEntity>().HasData(
            Coding,
            Debugging
        );
}
