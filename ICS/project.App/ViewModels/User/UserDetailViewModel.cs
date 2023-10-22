using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class UserDetailViewModel : ViewModelBase, IRecipient<UserEditMessage>
{
    private readonly IUserFacade _userFacade;
    private readonly INavigationService _navigationService;
    private readonly IAlertService _alertService;

    public Guid Id { get; set; }

    public string? Txt { get; private set; }

    public UserDetailModel? User { get; private set; }

    public UserDetailViewModel(
        IUserFacade userFacade,
        INavigationService navigationService,
        IMessengerService messengerService,
        IAlertService alertService)
        : base(messengerService)
    {
        _userFacade = userFacade;
        _navigationService = navigationService;
        _alertService = alertService;
    }

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        User = await _userFacade.GetAsync(Id);
        if (User is null)
        {
            var curId = await SecureStorage.Default.GetAsync("user_id");
            User = await _userFacade.GetAsync(new Guid(curId));
        }
        Txt = $"You have {User.Activities.Count} Activities";
}

    [RelayCommand]
    private async Task DeleteAsync()
    {
        if (User is not null)
        {
            try
            {
                await _userFacade.DeleteAsync(User.Id);
                MessengerService.Send(new UserDeleteMessage());
                _navigationService.SendBackButtonPressed();
            }
            catch (InvalidOperationException)
            {
                await _alertService.DisplayAsync("Error", "Error Occured");
            }
        }
    }

    [RelayCommand]
    private async Task GoToEditAsync()
    {
        await _navigationService.GoToAsync("/edit",
            new Dictionary<string, object?> { [nameof(UserEditViewModel.User)] = User });
    }

    [RelayCommand]
    private async Task LogoutAsync()
    {
        SecureStorage.Default.RemoveAll();
        await _navigationService.GoToAsync("//user");
    }

    public async void Receive(UserEditMessage message)
    {
        if (message.UserId == User?.Id)
        {
            await LoadDataAsync();
        }
    }
}
