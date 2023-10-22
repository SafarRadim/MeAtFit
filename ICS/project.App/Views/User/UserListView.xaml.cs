using project.App.ViewModels;

namespace project.App.Views.Others;

public partial class UserListView
{
    public UserListView(LoginViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}