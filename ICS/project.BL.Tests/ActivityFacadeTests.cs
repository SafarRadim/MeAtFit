using project.BL.Facades;
using project.BL.Models;
using project.Common.Tests;
using project.Common.Tests.Seeds;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using Xunit;
using Xunit.Abstractions;

namespace project.BL.Tests;

public class ActivityFacadeTests : FacadeTestsBase
{
    private readonly IActivityFacade _activityFacadeSUT;

    public ActivityFacadeTests(ITestOutputHelper output) : base(output)
    {
        _activityFacadeSUT = new ActivityFacade(UnitOfWorkFactory, ActivityModelMapper);
    }


    [Fact(Skip = "commented out")]
    public async Task Create_BasicActivity_WithoutTagActivities()
    {
        //Arrange
        var model = new ActivityDetailModel()
        {
            Name = "aaa",
            Description = "Testing activity 1",
            Start = new DateTime(2021, 1, 1, 8, 0, 0),
            End = new DateTime(2021, 1, 1, 16, 0, 0)
        };

        //Act
        var returnedModel = await _activityFacadeSUT.SaveAsync(model);
        
        FixIds(model, returnedModel);
        DeepAssert.Equal(model, returnedModel);
    }

    [Fact]
    public async Task GetAll_FromSeeded_DoesNotThrowAndContainsSeeded()
    {
        var listModel = ActivityModelMapper.MapToListModel(ActivitySeeds.Development_Without_TagActivities);

        var returnedModel = await _activityFacadeSUT.GetAsync();

        DeepAssert.Contains(listModel, returnedModel);
    }

    [Fact]
    public async Task Update_FromSeeded_DoesNotThrow()
    {
        var detailModel = ActivityModelMapper.MapToDetailModel(ActivitySeeds.Development_Without_TagActivities);
        detailModel.Description += " - updated";

        await _activityFacadeSUT.SaveAsync(detailModel);
    }

    [Fact]
    public async Task Update_FromSeeded_Delete()
    {
        var detailModel = ActivityModelMapper.MapToDetailModel(ActivitySeeds.Development_Without_TagActivities);
        detailModel.Description += " - updated";

        var returnedModel = await _activityFacadeSUT.SaveAsync(detailModel);

        DeepAssert.Equal(detailModel, returnedModel);
    }

    [Fact]
    public async Task DeleteById_FromSeeded_DoesNotThrow()
    {
        await _activityFacadeSUT.DeleteAsync(ActivitySeeds.Development_Without_TagActivities.Id);
    }

    private static void FixIds(ActivityDetailModel? expectedModel, ActivityDetailModel returnedModel)
    {
        if (expectedModel == null || returnedModel == null) return;

        returnedModel.Id = expectedModel.Id;
    }
}