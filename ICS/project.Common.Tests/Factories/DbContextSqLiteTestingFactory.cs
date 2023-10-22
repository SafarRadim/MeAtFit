using project.DAL;
using Microsoft.EntityFrameworkCore;

namespace project.Common.Tests.Factories;

public class DbContextSqLiteTestingFactory : IDbContextFactory<TrackerDbContext>
{
    private readonly string _databaseName;
    private readonly bool _seedTestingData;

    public DbContextSqLiteTestingFactory(string databaseName, bool seedTestingData = false)
    {
        _databaseName = databaseName;
        _seedTestingData = seedTestingData;
    }

    public TrackerDbContext CreateDbContext()
    {
        DbContextOptionsBuilder<TrackerDbContext> builder = new();
        builder.UseSqlite($"Data Source={_databaseName};Cache=Shared");

        return new TrackerTestingDbContext(builder.Options, _seedTestingData);
    }
}