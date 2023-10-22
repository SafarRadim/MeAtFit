using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Models;

namespace project.App.ViewModels;

public partial class TagListViewModel : ViewModelBase, IRecipient<TagEditMessage>, IRecipient<TagDeleteMessage>
{
    private readonly ITagFacade _tagFacade;
    private readonly IUserFacade _userFacade;
    private readonly INavigationService _navigationService;

    public IEnumerable<TagListModel> Tags { get; set; } = null!;

    public UserDetailModel? User { get; private set; }

    public TagListViewModel(
        ITagFacade tagFacade,
        INavigationService navigationService,
        IUserFacade userFacade,
        IMessengerService messengerService)
        : base(messengerService)
    {
        _tagFacade = tagFacade;
        _navigationService = navigationService;
        _userFacade = userFacade;
    }

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        Tags = await _tagFacade.GetAsync();
        var curId = await SecureStorage.Default.GetAsync("user_id");
        User = await _userFacade.GetAsync(new Guid(curId));
    }

    [RelayCommand]
    private async Task GoToCreateAsync()
    {
        await _navigationService.GoToAsync("/edit");
    }

    [RelayCommand]
    private async Task GoToDetailAsync(Guid id)
        => await _navigationService.GoToAsync<TagDetailViewModel>(
            new Dictionary<string, object?> { [nameof(TagDetailViewModel.Id)] = id });

    [RelayCommand]
    private async Task LogoutAsync()
    {
        SecureStorage.Default.RemoveAll();
        await _navigationService.GoToAsync("//user");
    }

    public async void Receive(TagEditMessage message)
    {
        await LoadDataAsync();
    }

    public async void Receive(TagDeleteMessage message)
    {
        await LoadDataAsync();
    }
}