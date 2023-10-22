using project.App.ViewModels;

namespace project.App.Views.Activity;

public partial class ActivityListView
{
    public ActivityListView(ActivityListViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}