using project.App.ViewModels;

namespace project.App.Views.Tag;

public partial class TagListView
{
    public TagListView(TagListViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}