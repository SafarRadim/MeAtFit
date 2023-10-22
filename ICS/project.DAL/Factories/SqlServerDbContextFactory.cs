using Microsoft.EntityFrameworkCore;

namespace project.DAL.Factories;

public class SqlServerDbContextFactory : IDbContextFactory<TrackerDbContext>
{
    private readonly bool _seedDemoData;
    private readonly DbContextOptionsBuilder<TrackerDbContext> _contextOptionsBuilder = new();

    public SqlServerDbContextFactory(string connectionString, bool seedDemoData = false)
    {
        _seedDemoData = seedDemoData;
        _contextOptionsBuilder.UseSqlServer(connectionString);
    }

    public TrackerDbContext CreateDbContext() => new(_contextOptionsBuilder.Options, _seedDemoData);
}