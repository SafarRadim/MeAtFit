using project.App.ViewModels;

namespace project.App.Views.Tag;

public partial class TagDetailView
{
    public TagDetailView(TagDetailViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}