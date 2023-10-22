using project.App.ViewModels;

namespace project.App.Views.Others;

public partial class WelcomeView
{
    public WelcomeView(UserListViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}