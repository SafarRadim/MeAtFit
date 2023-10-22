using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class TagDetailViewModel : ViewModelBase, IRecipient<TagEditMessage>
{
    private readonly ITagFacade _tagFacade;
    private readonly INavigationService _navigationService;

    public Guid Id { get; set; }

    public TagDetailModel? Tag { get; private set; }

    public TagDetailViewModel(
        ITagFacade tagFacade,
        INavigationService navigationService,
        IMessengerService messengerService)
        : base(messengerService)
    {
        _tagFacade = tagFacade;
        _navigationService = navigationService;
    }

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();
        Tag = await _tagFacade.GetAsync(Id);
    }

    [RelayCommand]
    private async Task DeleteAsync()
    {
        if (Tag is not null)
        {
            await _tagFacade.DeleteAsync(Tag.Id);
            MessengerService.Send(new TagDeleteMessage());
            _navigationService.SendBackButtonPressed();
        }
    }

    [RelayCommand]
    private async Task GoToEditAsync()
    {
        if (Tag is not null)
        {
            await _navigationService.GoToAsync("/edit",
                new Dictionary<string, object?>{ [nameof(TagEditViewModel.Tag)] = Tag with { } });
        }
    }

    public async void Receive(TagEditMessage message)
    {
        if (message.TagId == Tag?.Id)
        {
            await LoadDataAsync();
        }
    }
}