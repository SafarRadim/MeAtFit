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

public class ProjectFacadeTests : FacadeTestsBase
{
    private readonly IProjectFacade _projectFacadeSUT;

    public ProjectFacadeTests(ITestOutputHelper output) : base(output)
    {
        _projectFacadeSUT = new ProjectFacade(UnitOfWorkFactory, ProjectModelMapper);
    }

    [Fact (Skip = "commented out")]
    public async Task Create_Project_WithoutUsersAndActivities()
    {
        //Arrange
        var model = new ProjectDetailModel()
        {
            Name = "Testing activity 1",
            Description = "Desc"
        };

        //Act
        var returnedModel = await _projectFacadeSUT.SaveAsync(model);
        
        FixIds(model, returnedModel);
        DeepAssert.Equal(model, returnedModel);
    }

    [Fact]
    public async Task Create_Project()
    {
        var model = new ProjectDetailModel()
        {
            Name = "Testing activity 1",
            Activities = new ObservableCollection<ActivityListModel>()
            {
                new ActivityListModel()
                {
                    Name = "Holand"
                },
                new ActivityListModel()
                {
                    Name = "Kern 2"
                }
            },
            Description = "desc"
        };
        await Assert.ThrowsAnyAsync<InvalidOperationException>(() => _projectFacadeSUT.SaveAsync(model));
    }

    [Fact]
    public async Task GetById_FromSeeded_DoesNotThrowAndEqualsSeeded()
    {
        var detailModel = ProjectModelMapper.MapToDetailModel(ProjectSeeds.newGenerationTracker);

        var returnedModel = await _projectFacadeSUT.GetAsync(detailModel.Id);

        DeepAssert.Equal(detailModel, returnedModel);
    }

    [Fact]
    public async Task GetAll_FromSeeded_DoesNotThrowAndContainsSeeded()
    {
        var listModel = ProjectModelMapper.MapToListModel(ProjectSeeds.TrackerSoftware);

        var returnedModel = await _projectFacadeSUT.GetAsync();

        DeepAssert.Contains(listModel, returnedModel);
    }


    [Fact(Skip = "could be caused by DAL/BL changes")]
    public async Task Update_FromSeeded_DoesThorw()
    {
        var detailModel = ProjectModelMapper.MapToDetailModel(ProjectSeeds.TrackerSoftware);
        detailModel.Name += " - updated";

        await Assert.ThrowsAnyAsync<InvalidOperationException>(() => _projectFacadeSUT.SaveAsync(detailModel));
    }

    [Fact]
    public async Task DeleteById_FromSeeded_DoesNotThrow()
    {
        await _projectFacadeSUT.DeleteAsync(ProjectSeeds.TrackerSoftware.Id);
    }

    private static void FixIds(ProjectDetailModel? expectedModel, ProjectDetailModel returnedModel)
    {
        if (expectedModel == null || returnedModel == null) return;

        returnedModel.Id = expectedModel.Id;

        foreach (var activity in returnedModel.Activities)
        {
            activity.Id = expectedModel.Activities.First(a => a.Name == activity.Name).Id;
        }
    }
}