using Microsoft.EntityFrameworkCore.Design;

namespace project.DAL.Factories;

public class DesignTimeDbContextFactory : IDesignTimeDbContextFactory<TrackerDbContext>
{
    private readonly DbContextSqLiteFactory _dbContextSqLiteFactory;
    private const string ConnectionString = $"Data Source=Tracker;Cache=Shared";

    public DesignTimeDbContextFactory()
    {
        _dbContextSqLiteFactory = new DbContextSqLiteFactory(ConnectionString);
    }

    public TrackerDbContext CreateDbContext(string[] args) => _dbContextSqLiteFactory.CreateDbContext();
}