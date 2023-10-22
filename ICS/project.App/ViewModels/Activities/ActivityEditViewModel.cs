using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.Input;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(Activity), nameof(Activity))]
public partial class ActivityEditViewModel : ViewModelBase
{
    private readonly IActivityFacade _activityFacade;
    private readonly IProjectFacade _projectFacade;
    private readonly INavigationService _navigationService;

    public ActivityDetailModel Activity { get; init; } = ActivityDetailModel.Empty;
    public ObservableCollection<ProjectListModel> Projects { get; set; } = new();
    public ProjectListModel ProjectSelected { get; set; }

    public ActivityEditViewModel(
        IActivityFacade activityFacade,
        IProjectFacade projectFacade,
        INavigationService navigationService,
        IMessengerService messengerService)
        : base(messengerService)
    {
        _activityFacade = activityFacade;
        _navigationService = navigationService;
        _projectFacade = projectFacade;
    }

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        var projects = await _projectFacade.GetAsync();
        Projects.Add(GetNoneProjectListModel());
        ProjectSelected = GetNoneProjectListModel();
        foreach (var project in projects)
        {
            Projects.Add(project);
            if (Activity.ProjectId == project.Id)
            {
                ProjectSelected = project;
            }
        }
    }

    [RelayCommand]
    private async Task SaveAsync()
    {

        var curId = await SecureStorage.Default.GetAsync("user_id");
        Activity.UserId = new Guid(curId);

        if (ProjectSelected.Id == Guid.Empty)
        {
            Activity.ProjectId = null;
        } else
        {
            Activity.ProjectId = ProjectSelected.Id;
        }

        var newItem = await _activityFacade.SaveAsync(Activity with { TagActivities = default! });

        MessengerService.Send(new ActivityEditMessage { ActivityId = Activity.Id });
        _navigationService.SendBackButtonPressed();
    }

    private ProjectListModel GetNoneProjectListModel()
    {
        return new ProjectListModel
        {
            Id = Guid.Empty,
            Name = "No project"
        };
    }
}