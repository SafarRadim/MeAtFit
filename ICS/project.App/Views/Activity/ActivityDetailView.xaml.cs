using project.App.ViewModels;

namespace project.App.Views.Activity;

public partial class ActivityDetailView
{
    public ActivityDetailView(ActivityDetailViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}