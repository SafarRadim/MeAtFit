using CommunityToolkit.Mvvm.Input;
using project.App.Services;
using project.App.ViewModels;

namespace project.App.Shells;

public partial class AppShell
{
    private readonly INavigationService _navigationService;

    public AppShell(INavigationService navigationService)
    {
        _navigationService = navigationService;

        InitializeComponent();
    }

    [RelayCommand]
    private async Task GoToProjectsAsync()
        => await _navigationService.GoToAsync<ProjectListViewModel>();

    [RelayCommand]
    private async Task GoToActivitiesAsync()
        => await _navigationService.GoToAsync<ActivityListViewModel>();

    [RelayCommand]
    private async Task GoToWelcomeAsync()
        => await _navigationService.GoToAsync<LoginViewModel>();

    [RelayCommand]
    private async Task GoToLoginAsync()
        => await _navigationService.GoToAsync<LoginViewModel>();

    [RelayCommand]
    private async Task GoToTagsAsync()
        => await _navigationService.GoToAsync<TagListViewModel>();
    [RelayCommand]
    private async Task GoToUserDetailAsync()
        => await _navigationService.GoToAsync<UserDetailViewModel>();
}