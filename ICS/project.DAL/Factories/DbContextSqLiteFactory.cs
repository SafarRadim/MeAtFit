using Microsoft.EntityFrameworkCore;

namespace project.DAL.Factories;

public class DbContextSqLiteFactory : IDbContextFactory<TrackerDbContext>
{
    private readonly bool _seedTestingData;
    private readonly DbContextOptionsBuilder<TrackerDbContext> _contextOptionsBuilder = new();

    public DbContextSqLiteFactory(string databaseName, bool seedTestingData = false)
    {
        _seedTestingData = seedTestingData;
        _contextOptionsBuilder.UseSqlite($"Data Source={databaseName};Cache=Shared");
    }

    public TrackerDbContext CreateDbContext() => new(_contextOptionsBuilder.Options, true);
}