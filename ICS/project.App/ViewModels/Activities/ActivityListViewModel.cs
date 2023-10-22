using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Models;

namespace project.App.ViewModels;

public partial class ActivityListViewModel : ViewModelBase, IRecipient<ActivityEditMessage>, IRecipient<ActivityDeleteMessage>
{
    private readonly IActivityFacade _activityFacade;
    private readonly IUserFacade _userFacade;
    private readonly INavigationService _navigationService;

    public IEnumerable<ActivityListModel> Activities { get; set; } = null!;

    public UserDetailModel? User { get; private set; }

    public DateTime FilterDateStart { get; set; } = DateTime.Today;
    public DateTime FilterDateEnd { get; set; } = DateTime.Today;

    public ActivityListViewModel(
        IActivityFacade activityFacade,
        IUserFacade userFacade,
        INavigationService navigationService,
        IMessengerService messengerService)
        : base(messengerService)
    {
        _activityFacade = activityFacade;
        _navigationService = navigationService;
        _userFacade = userFacade;
    }

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();
        string curId = await SecureStorage.Default.GetAsync("user_id");
        User = await _userFacade.GetAsync(new Guid(curId));
        FilterDateStart = DateTime.Today;
        FilterDateEnd = DateTime.Today;
        Activities = await _activityFacade.GetActivityListFilteredAsync(new Guid(curId), null, null);
    }

    [RelayCommand]
    private async Task GoToCreateAsync()
    {
        await _navigationService.GoToAsync("/edit");
    }

    [RelayCommand]
    private async Task GoToDetailAsync(Guid id)
    {
        await _navigationService.GoToAsync<ActivityDetailViewModel>(
            new Dictionary<string, object?> { [nameof(ActivityDetailViewModel.Id)] = id });
    }

    [RelayCommand]
    private async Task FilterActivitiesWeek()
    {
        FilterDateStart = DateTime.Today.AddDays(-7);
        await FilterActivities();
    }

    [RelayCommand]
    private async Task FilterActivitiesMonth()
    {
        FilterDateStart = DateTime.Today.AddMonths(-1);
        await FilterActivities();
    }

    [RelayCommand]
    private async Task FilterActivitiesYear()
    {
        FilterDateStart = DateTime.Today.AddMonths(-12);
        await FilterActivities();
    }

    [RelayCommand]
    private async Task FilterActivities()
    {
        await base.LoadDataAsync();
        string curId = await SecureStorage.Default.GetAsync("user_id");
        User = await _userFacade.GetAsync(new Guid(curId));
        Activities = await _activityFacade.GetActivityListFilteredAsync(new Guid(curId), FilterDateStart, null);
    }

    [RelayCommand]
    private async Task LogoutAsync()
    {
        SecureStorage.Default.RemoveAll();
        await _navigationService.GoToAsync("//user");
    }

    public async void Receive(ActivityEditMessage message)
    {
        await LoadDataAsync();
    }

    public async void Receive(ActivityDeleteMessage message)
    {
        await LoadDataAsync();
    }
}