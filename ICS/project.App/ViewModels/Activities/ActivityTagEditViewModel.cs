using System.Collections.ObjectModel;
using System.Reflection.Metadata;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using Microsoft.EntityFrameworkCore;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Mappers;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class ActivityTagEditViewModel : ViewModelBase, IRecipient<TagActivityAddMessage>
{
    private readonly ITagFacade _tagFacade;
    private readonly IActivityFacade _activityFacade;
    private readonly ITagActivityFacade _tagActivityFacade;
    private readonly ITagActivityModelMapper _tagActivityModelMapper;

    private readonly INavigationService _navigationService;

    public Guid Id { get; set; }
    public ActivityDetailModel? Activity { get; set; }
    public ObservableCollection<TagListModel> Tags { get; set; } = new();
    public TagListModel TagSelected { get; set; }
    public TagActivityDetailModel? ActivityTagNew { get; private set; }
    public ActivityTagEditViewModel(
        ITagFacade tagFacade,
        IActivityFacade activityFacade,
        ITagActivityFacade tagActivityFacade,
        ITagActivityModelMapper tagActivityModelMapper,
        INavigationService navigationService,
        IMessengerService messengerService)
        : base(messengerService)
    {
        _tagFacade = tagFacade;
        _activityFacade = activityFacade;
        _tagActivityFacade = tagActivityFacade;
        _tagActivityModelMapper = tagActivityModelMapper;
        _navigationService = navigationService;
    }

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();
        Tags = new();
        var tags = await _tagFacade.GetAsync();
        foreach (var tag in tags)
        {
            Tags.Add(tag);
        }

        Activity = await _activityFacade.GetAsync(Id);
        ActivityTagNew = GetActivityTagNew();
    }

    [RelayCommand]
    private async Task AddNewTagAsync()
    {
        if (ActivityTagNew is not null
            && TagSelected is not null
            && Activity is not null)
        {
            _tagActivityModelMapper.MapToExistingDetailModel(ActivityTagNew, TagSelected);
            try
            {
                await _tagActivityFacade.SaveAsync(ActivityTagNew, Activity.Id);
                Activity.TagActivities.Add(_tagActivityModelMapper.MapToListModel(ActivityTagNew));
            }
            catch (DbUpdateException ex)
            {

            }
            finally 
            {
                ActivityTagNew = GetActivityTagNew();
                MessengerService.Send(new TagActivityAddMessage());
            }
        }
    }

    [RelayCommand]
    private async Task RemoveTagAsync(TagActivityListModel model)
    {
        if (Activity is not null)
        {
            await _tagActivityFacade.DeleteAsync(model.Id);
            Activity.TagActivities.Remove(model);

            MessengerService.Send(new ActivityTagDeleteMessage());
        }
    }

    public async void Receive(TagActivityAddMessage message)
    {
        await LoadDataAsync();
    }

    private TagActivityDetailModel GetActivityTagNew()
    {
        var firstTag = Tags.First();
        return new()
        {
            Id = Guid.NewGuid(),
            TagId = firstTag.Id,
            TagName = firstTag.TagName
        };
    }
}
