using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using project.App.Services;
using project.BL.Facades;
using project.BL.Models;
namespace project.App.ViewModels;

[QueryProperty(nameof(Project), nameof(Project))]
public partial class ProjectEditViewModel : ViewModelBase
{
    private readonly IProjectFacade _projectFacade;
    private readonly INavigationService _navigationService;

    public ProjectDetailModel Project { get; set; } = ProjectDetailModel.Empty;

    public ProjectEditViewModel(
        IProjectFacade projectFacade,
        INavigationService navigationService,
        IMessengerService messengerService)
        : base(messengerService)
    {
        _projectFacade = projectFacade;
        _navigationService = navigationService;

    }


    [RelayCommand]
    private async Task SaveAsync()
    {
        await _projectFacade.SaveAsync(Project with { Users = default! });
        MessengerService.Send(new ProjectEditMessage { ProjectId = Project.Id });

        _navigationService.SendBackButtonPressed();
    }

    private async Task ReloadDataAsync()
    {
        Project = await _projectFacade.GetAsync(Project.Id)
                 ?? ProjectDetailModel.Empty;
    }
}
