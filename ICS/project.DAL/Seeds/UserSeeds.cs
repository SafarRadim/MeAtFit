using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.DAL.Seeds;

public static class UserSeeds
{
    public static readonly UserEntity Jozef = new()
    {
        Id = Guid.Parse("d8f5eb07-5d42-4e2f-bbf3-fc6824e6c2ed"),
        Name = "Jozef",
        Surname = "Placko",
        Email = "jozefplacko@gmail.com",
        ImageUrl = "https://img.topky.sk/1280px/2761489.jpg"
    };

    static UserSeeds()
    {
        Jozef.Activities.Add(ActivitySeeds.Development);
    }

    public static void Seed(this ModelBuilder modelBuilder) =>
        modelBuilder.Entity<UserEntity>().HasData(
            Jozef with { Activities = Array.Empty<ActivityEntity>() }
        );
}


