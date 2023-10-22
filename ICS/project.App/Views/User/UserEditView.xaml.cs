using project.App.ViewModels;

namespace project.App.Views.Others;

public partial class UserEditView
{
    public UserEditView(UserEditViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}