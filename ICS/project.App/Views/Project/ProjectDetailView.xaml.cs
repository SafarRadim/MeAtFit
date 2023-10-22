using project.App.ViewModels;

namespace project.App.Views.Project;

public partial class ProjectDetailView
{
    public ProjectDetailView(ProjectDetailViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}