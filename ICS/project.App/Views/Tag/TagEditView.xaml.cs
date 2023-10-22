using project.App.ViewModels;

namespace project.App.Views.Tag;

public partial class TagEditView
{
    public TagEditView(TagEditViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}