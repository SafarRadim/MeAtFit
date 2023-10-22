using project.App.Models;
using project.App.ViewModels;
using project.App.Views.Project;
using project.App.Views.Activity;
using project.App.Views.Others;
using project.App.Views.Tag;


namespace project.App.Services;

public class NavigationService : INavigationService
{
    public IEnumerable<RouteModel> Routes { get; } = new List<RouteModel>
    {
        new("//user", typeof(UserListView), typeof(LoginViewModel)),
        new("//user/detail", typeof(UserDetailView), typeof(UserDetailViewModel)),

        new("//user/edit", typeof(UserEditView), typeof(UserEditViewModel)),
        new("//user/detail/edit", typeof(UserEditView), typeof(UserEditViewModel)),

        new("//projects", typeof(ProjectListView), typeof(ProjectListViewModel)),
        new("//projects/detail", typeof(ProjectDetailView), typeof(ProjectDetailViewModel)),

        new("//projects/edit/", typeof(ProjectEditView), typeof(ProjectDetailViewModel)),
        new("//projects/detail/edit/", typeof(ProjectEditView), typeof(ProjectDetailViewModel)),

        new("//activities", typeof(ActivityListView), typeof(ActivityListViewModel)),
        new("//activities/detail", typeof(ActivityDetailView), typeof(ActivityDetailViewModel)),

        new("//activities/edit", typeof(ActivityEditView), typeof(ActivityEditViewModel)),
        new("//activities/detail/edit", typeof(ActivityEditView), typeof(ActivityEditViewModel)),

        new("//activities/detail/tags", typeof(ActivityTagEditView), typeof(ActivityTagEditViewModel)),

        new("//tags", typeof(TagListView), typeof(TagListViewModel)),
        new("//tags/detail", typeof(TagDetailView), typeof(TagDetailViewModel)),

        new("//tags/edit", typeof(TagEditView), typeof(TagEditViewModel)),
        new("//tags/detail/edit", typeof(TagEditView), typeof(TagEditViewModel)),
    };
    public async Task GoToAsync<TViewModel>()
        where TViewModel : IViewModel
    {
        var route = GetRouteByViewModel<TViewModel>();
        await Shell.Current.GoToAsync(route);
    }
    public async Task GoToAsync<TViewModel>(IDictionary<string, object?> parameters)
        where TViewModel : IViewModel
    {
        var route = GetRouteByViewModel<TViewModel>();
        await Shell.Current.GoToAsync(route, parameters);
    }

    public async Task GoToAsync(string route)
        => await Shell.Current.GoToAsync(route);

    public async Task GoToAsync(string route, IDictionary<string, object?> parameters)
        => await Shell.Current.GoToAsync(route, parameters);

    public bool SendBackButtonPressed()
        => Shell.Current.SendBackButtonPressed();

    private string GetRouteByViewModel<TViewModel>()
        where TViewModel : IViewModel
        => Routes.First(route => route.ViewModelType == typeof(TViewModel)).Route;
}