using project.Common.Tests.Seeds;
using project.DAL;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Diagnostics;

namespace project.Common.Tests;

public class TrackerTestingDbContext : TrackerDbContext
{
    private readonly bool _seedTestingData;

    public TrackerTestingDbContext(DbContextOptions contextOptions, bool seedTestingData = false)
        : base(contextOptions, seedDemoData:false)
    {
        _seedTestingData = seedTestingData;
    }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);

        if (_seedTestingData)
        {
            ActivitySeeds.Seed(modelBuilder);
            ProjectSeeds.Seed(modelBuilder);
            TagSeeds.Seed(modelBuilder);
            UserSeeds.Seed(modelBuilder);
            TagActivitySeeds.Seed(modelBuilder);
            UserProjectSeeds.Seed(modelBuilder);
        }
    }
    protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
    {
        optionsBuilder.ConfigureWarnings(warnings => warnings.Ignore(CoreEventId.InvalidIncludePathError));
        base.OnConfiguring(optionsBuilder);
    }
}