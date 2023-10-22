using project.App.ViewModels;

namespace project.App.Views.Project;

public partial class ProjectEditView
{
    public ProjectEditView(ProjectEditViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}