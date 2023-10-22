using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Mappers;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(Id), nameof(Id))]
public partial class ProjectDetailViewModel : ViewModelBase, IRecipient<ProjectEditMessage>, IRecipient<UserProjectAddMessage>, IRecipient<UserProjectRemoveMessage>
{
    private readonly IProjectFacade _projectFacade;
    private readonly IUserFacade _userFacade;
    private readonly IUserProjectFacade _userProjectFacade;
    private readonly IUserProjectModelMapper _userProjectModelMapper;

    private readonly INavigationService _navigationService;

    public Guid Id { get; set; }

    public ProjectDetailModel? Project { get; private set; }
    public UserDetailModel? User { get; private set; }
    public ObservableCollection<UserListModel> Users { get; set; } = new();
    public UserProjectDetailModel? UserProjectNew { get; private set; }
    public UserProjectListModel? UserProject { get; private set; }

    public bool JoinVisible { get; private set; } = true;
    public bool LeaveVisible { get; private set; } = false;

    public ProjectDetailViewModel(
        IProjectFacade projectFacade,
        IUserFacade userFacade,
        IUserProjectFacade userProjectFacade,
        INavigationService navigationService,
        IUserProjectModelMapper userProjectModelMapper,
        IMessengerService messengerService)
        : base(messengerService)
    {
        _projectFacade = projectFacade;
        _userFacade = userFacade;
        _userProjectFacade = userProjectFacade;
        _userProjectModelMapper = userProjectModelMapper;
        _navigationService = navigationService;
    }

    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();
        Project = await _projectFacade.GetAsync(Id);

        var curId = await SecureStorage.Default.GetAsync("user_id");
        User = await _userFacade.GetAsync(new Guid(curId));

        var users = await _userFacade.GetAsync();
        foreach (var user in users)
        {
            Users.Add(user);
        }
        UserProjectNew = GetUserProjectNew();

        foreach (var project in Project.Users)
        {
            if (User.Id != project.UserId) continue;
            UserProject = project;
            JoinVisible = false;
            LeaveVisible = true;
            break;
        }
    }

    [RelayCommand]
    private async Task DeleteAsync()
    {
        if (Project is not null)
        {
            await _projectFacade.DeleteAsync(Project.Id);
            MessengerService.Send(new ProjectDeleteMessage());
            _navigationService.SendBackButtonPressed();
        }
    }


    [RelayCommand]
    private async Task GoToEditAsync()
    {
        if (Project is not null)
        {
            await _navigationService.GoToAsync("/edit",
                new Dictionary<string, object?> { [nameof(ProjectEditViewModel.Project)] = Project with { } });
        }
    }

    [RelayCommand]
    private async Task JoinClickedAsync()
    {
        if (User is not null
            && Project is not null
            && UserProjectNew is not null)
        {
            var listModel = GetListModel(User);
            _userProjectModelMapper.MapToExistingDetailModel(UserProjectNew, listModel);
            await _userProjectFacade.SaveAsync(UserProjectNew, Project.Id);

            Project.Users.Add(_userProjectModelMapper.MapToListModel(UserProjectNew));

            UserProjectNew = GetUserProjectNew();
            JoinVisible = false;
            LeaveVisible = true;

            MessengerService.Send(new UserProjectAddMessage());
        }
    }

    [RelayCommand]
    private async Task LeaveClickedAsync()
    {
        if (User is not null
            && Project is not null
            && UserProject is not null)
        {
            await _userProjectFacade.DeleteAsync(UserProject.Id);
            Project.Users.Remove(UserProject);
            
            JoinVisible = true;
            LeaveVisible = false;

            MessengerService.Send(new UserProjectRemoveMessage());
        }
    }

    [RelayCommand]
    private async Task LogoutAsync()
    {
        SecureStorage.Default.RemoveAll();
        await _navigationService.GoToAsync("//user");
    }

    private UserProjectDetailModel GetUserProjectNew()
    {
        var userFirst = Users.First();
        return new UserProjectDetailModel
        {
            Id = Guid.NewGuid(),
            UserId = userFirst.Id,
            UserName = userFirst.Name
        };
    }

    private UserListModel GetListModel(UserDetailModel userDetail)
    {
        foreach (var user in Users)
        {
            if (user.Id == userDetail.Id)
            {
                return user;
            }
        }
        
        return null;
    }

    public async void Receive(ProjectEditMessage message)
    {
        if (message.ProjectId == Project?.Id)
        {
            await LoadDataAsync();
        }
    }
    public async void Receive(UserProjectAddMessage message)
    {
        await LoadDataAsync();
    }

    public async void Receive(UserProjectRemoveMessage message)
    {
        await LoadDataAsync();
    }
}
