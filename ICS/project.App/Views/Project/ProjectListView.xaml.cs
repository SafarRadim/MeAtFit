using project.App.ViewModels;

namespace project.App.Views.Project;

public partial class ProjectListView
{
    public ProjectListView(ProjectListViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}