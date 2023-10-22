using CommunityToolkit.Mvvm.Input;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(Tag), nameof(Tag))]
public partial class TagEditViewModel : ViewModelBase
{
    private readonly ITagFacade _tagFacade;
    private readonly INavigationService _navigationService;

    public TagDetailModel Tag { get; set; } = TagDetailModel.Empty;

    public TagEditViewModel(
        ITagFacade tagFacade,
        INavigationService navigationService,
        IMessengerService messengerService)
        : base(messengerService)
    {
        this._tagFacade = tagFacade;
        this._navigationService = navigationService;

    }

    [RelayCommand]
    private async Task SaveAsync()
    {
        await _tagFacade.SaveAsync(Tag);
        MessengerService.Send(new TagEditMessage { TagId = Tag.Id });

        _navigationService.SendBackButtonPressed();
    }
}
