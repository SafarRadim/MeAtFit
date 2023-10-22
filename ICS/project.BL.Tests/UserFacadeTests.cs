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
using Microsoft.EntityFrameworkCore.Diagnostics;

namespace project.BL.Tests;

public class UserFacadeTests : FacadeTestsBase
{
    private readonly IUserFacade _userFacadeSUT;

    public UserFacadeTests(ITestOutputHelper output) : base(output)
    {
        _userFacadeSUT = new UserFacade(UnitOfWorkFactory, UserModelMapper);
    }


    [Fact]
    public async Task Create_User_Without_Anything()
    {
        //Arrange
        var model = new UserDetailModel()
        {
            Name = "Dezider",
            Surname = "kralik",
            Email = "dezider@gmail.com"
        };

        //Act
        var returnedModel = await _userFacadeSUT.SaveAsync(model);

        FixIds(model, returnedModel);
        DeepAssert.Equal(model, returnedModel);
    }

    [Fact]
    public async Task Create_User_With_Activities()
    {
        //Arrange
        var model = new UserDetailModel()
        {
            Name = "Dezider",
            Surname = "kralik",
            Email = "aaaaaaaaa@g.sk",
            Activities = new ObservableCollection<ActivityListModel>()
            {
                new ActivityListModel()
                {
                    Name = "Description 1"
                },
                new ActivityListModel()
                {
                    Name = "Description 2"
                }
            }
        };

        await Assert.ThrowsAnyAsync<InvalidOperationException>(() => _userFacadeSUT.SaveAsync(model));
    }

    [Fact(Skip = "commented out")]
    public async Task GetById_FromSeeded_DoesNotThrowAndEqualsSeeded()
    {
        var detailModel = UserModelMapper.MapToDetailModel(UserSeeds.Jozef);

        var returnedModel = await _userFacadeSUT.GetAsync(detailModel.Id);

        FixIds(detailModel, returnedModel);

        DeepAssert.Equal(detailModel, returnedModel);
    }

    [Fact]
    public async Task GetAll_FromSeeded_DoesNotThrowAndContainsSeeded()
    {
        var listModel = UserModelMapper.MapToListModel(UserSeeds.Jozef);

        var returnedModel = await _userFacadeSUT.GetAsync();

        DeepAssert.Contains(listModel, returnedModel);
    }

    [Fact(Skip = "commented out")]
    public async Task Update_FromSeeded_DoesNotThrow()
    {
        var detailModel = UserModelMapper.MapToDetailModel(UserSeeds.Jozef);
        detailModel.Name += " - updated";

        await _userFacadeSUT.SaveAsync(detailModel);
    }

    [Fact(Skip = "commented out")]
    public async Task Update_RemoveActivity_FromSeeded()
    {
        var detailModel = UserModelMapper.MapToDetailModel(UserSeeds.Jozef);
        detailModel.Activities.Clear();

        await _userFacadeSUT.SaveAsync(detailModel);

        var returnedModel = await _userFacadeSUT.GetAsync(detailModel.Id);
        DeepAssert.Equal(UserModelMapper.MapToDetailModel(UserSeeds.Jozef), returnedModel);
    }

    [Fact]
    public async Task DeleteById_FromSeeded_DoesNotThrow()
    {
        await _userFacadeSUT.DeleteAsync(UserSeeds.Jozef.Id);
    }

    private static void FixIds(UserDetailModel expectedModel, UserDetailModel? returnedModel)
    {
        if (returnedModel is null)
        {
            throw new ArgumentNullException(nameof(returnedModel));
        }

        returnedModel.Id = expectedModel.Id;

        foreach (var activity in returnedModel.Activities)
        {
            activity.Id = expectedModel.Activities.First(a => a.Name == activity.Name).Id;
        }
    }
}