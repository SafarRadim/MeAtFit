using project.App.ViewModels;

namespace project.App.Views.Others;

public partial class UserDetailView
{
    public UserDetailView(UserDetailViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}