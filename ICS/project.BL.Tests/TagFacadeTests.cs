using project.BL.Facades;
using project.BL.Models;
using project.Common.Tests;
using project.Common.Tests.Seeds;
using Xunit;
using Xunit.Abstractions;

namespace project.BL.Tests;

public class TagFacadeTests : FacadeTestsBase
{
    private readonly ITagFacade _tagFacadeSUT;

    public TagFacadeTests(ITestOutputHelper output) : base(output)
    {
        _tagFacadeSUT = new TagFacade(UnitOfWorkFactory, TagModelMapper);
    }


    [Fact]
    public async Task Create_Tag_Without_Activities()
    {
        //Arrange
        var model = new TagDetailModel()
        {
            TagName = "Tag 1"
        };

        //Act
        var returnedModel = await _tagFacadeSUT.SaveAsync(model);
        
        FixIds(model, returnedModel);
        DeepAssert.Equal(model, returnedModel);
    }


    [Fact]
    public async Task GetById_FromSeeded_DoesNotThrowAndEqualsSeeded()
    {
        var detailModel = TagModelMapper.MapToDetailModel(TagSeeds.Debugging);

        var returnedModel = await _tagFacadeSUT.GetAsync(detailModel.Id);

        FixIds(detailModel, returnedModel);

        DeepAssert.Equal(detailModel, returnedModel);
    }

    [Fact]
    public async Task GetAll_FromSeeded_DoesNotThrowAndContainsSeeded()
    {
        var listModel = TagModelMapper.MapToListModel(TagSeeds.Coding);

        var returnedModel = await _tagFacadeSUT.GetAsync();

        DeepAssert.Contains(listModel, returnedModel);
    }

    [Fact]
    public async Task Update_FromSeeded_DoesNotThrow()
    {
        var detailModel = TagModelMapper.MapToDetailModel(TagSeeds.Coding);
        detailModel.TagName += " - updated";

        await _tagFacadeSUT.SaveAsync(detailModel);
    }

    [Fact]
    public async Task DeleteById_FromSeeded_DoesNotThrow()
    {
        await _tagFacadeSUT.DeleteAsync(TagSeeds.Coding.Id);
    }

    private static void FixIds(TagDetailModel expectedModel, TagDetailModel? returnedModel)
    {
        if (returnedModel == null)
            return;

        returnedModel.Id = expectedModel.Id;

        if (returnedModel.Activites != null)
            foreach (var activity in returnedModel.Activites)
                activity.Id = expectedModel.Id;
    }
}