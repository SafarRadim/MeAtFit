using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.Common.Tests.Seeds;

public static class UserSeeds
{
    public static readonly UserEntity UserEmptyEntity = new()
    {
        Id = default,
        Name = default!,
        Surname = default!,
        Email = default!,
        ImageUrl = default
    };

    public static readonly UserEntity Jozef = new()
    {
        Id = Guid.Parse("d8f5eb07-5d42-4e2f-bbf3-fc6824e6c2ed"),
        Name = "Jozef",
        Surname = "Placko",
        Email = "jozefplacko@gmail.com",
        ImageUrl = "https://upload.wikimedia.org/wikipedia/commons/a/ae/Jozef_Haller.jpg"
    };

    static UserSeeds()
    {
        Jozef.Activities.Add(ActivitySeeds.Development_Without_TagActivities);
    }

    public static void Seed(this ModelBuilder modelBuilder) =>
        modelBuilder.Entity<UserEntity>().HasData(
            Jozef with { Activities = Array.Empty<ActivityEntity>() }
        );
}