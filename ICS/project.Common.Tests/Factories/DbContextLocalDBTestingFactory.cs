using project.DAL;
using Microsoft.EntityFrameworkCore;
using project.Common.Tests;

namespace project.Common.Tests.Factories;

public class DbContextLocalDBTestingFactory : IDbContextFactory<TrackerDbContext>
{
    private readonly string _databaseName;
    private readonly bool _seedTestingData;

    public DbContextLocalDBTestingFactory(string databaseName, bool seedTestingData = false)
    {
        _databaseName = databaseName;
        _seedTestingData = seedTestingData;
    }
    public TrackerDbContext CreateDbContext()
    {
        DbContextOptionsBuilder<TrackerTestingDbContext> builder = new();
        builder.UseSqlServer($"Data Source=(LocalDB)\\MSSQLLocalDB;Initial Catalog = {_databaseName};MultipleActiveResultSets = True;Integrated Security = True; ");

        // contextOptionsBuilder.LogTo(System.Console.WriteLine); //Enable in case you want to see tests details, enabled may cause some inconsistencies in tests
        // builder.EnableSensitiveDataLogging();

        return new TrackerTestingDbContext(builder.Options, _seedTestingData);
    }
}