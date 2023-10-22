using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class ActivityDetailViewModel : ViewModelBase, IRecipient<ActivityEditMessage>
{
    private readonly IActivityFacade _activityFacade;
    private readonly IUserFacade _userFacade;
    private readonly IProjectFacade _projectFacade;
    private readonly INavigationService _navigationService;

    public UserDetailModel? User { get; private set; }
    public Guid Id { get; set; }

    public ActivityDetailModel? Activity { get; private set; }
    public ProjectDetailModel? Project { get; private set; }

    public ActivityDetailViewModel(
        IActivityFacade activityFacade,
        IUserFacade userFacade,
        IProjectFacade projectFacade,
        INavigationService navigationService,
        IMessengerService messengerService)
        : base(messengerService)
    {
        _activityFacade = activityFacade;
        _navigationService = navigationService;
        _userFacade = userFacade;
        _projectFacade = projectFacade;
    }

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();
        string curId = await SecureStorage.Default.GetAsync("user_id");
        User = await _userFacade.GetAsync(new Guid(curId));
        Activity = await _activityFacade.GetAsync(Id);
        Project = Activity.ProjectId == Guid.Empty ? GetEmptyProject() : await _projectFacade.GetAsync((Guid)Activity.ProjectId);
    }

    [RelayCommand]
    private async Task DeleteAsync()
    {
        if (Activity is not null)
        {
            await _activityFacade.DeleteAsync(Activity.Id);
            MessengerService.Send(new ActivityDeleteMessage());
            _navigationService.SendBackButtonPressed();
        }
    }

    [RelayCommand]
    private async Task GoToEditAsync()
    {
        await _navigationService.GoToAsync("/edit",
            new Dictionary<string, object?> { [nameof(ActivityEditViewModel.Activity)] = Activity });
    }

    [RelayCommand]
    private async Task GoToTagsAsync()
    {
        await _navigationService.GoToAsync("/tags",
            new Dictionary<string, object?> { [nameof(ActivityTagEditViewModel.Id)] = Id });
    }

    [RelayCommand]
    private async Task LogoutAsync()
    {
        SecureStorage.Default.RemoveAll();
        await _navigationService.GoToAsync("//user");
    }

    public async void Receive(ActivityEditMessage message)
    {
        if (message.ActivityId == Activity?.Id)
        {
            await LoadDataAsync();
        }
    }

    private ProjectDetailModel GetEmptyProject()
    {
        var emptyProject = ProjectDetailModel.Empty;
        emptyProject.Name = "No Project";
        return emptyProject;
    }
}
