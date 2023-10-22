using CommunityToolkit.Mvvm.ComponentModel;
using project.App.Services;

namespace project.App.ViewModels;

public abstract class ViewModelBase : ObservableRecipient, IViewModel
{
    private bool _isRefreshRequired = true;

    protected readonly IMessengerService MessengerService;

    protected ViewModelBase(IMessengerService messengerService)
        : base(messengerService.Messenger)
    {
        MessengerService = messengerService;
        IsActive = true;
    }

    public async Task OnAppearingAsync()
    {
        if (_isRefreshRequired)
        {
            await LoadDataAsync();

            _isRefreshRequired = true;
        }
    }

    protected virtual Task LoadDataAsync()
        => Task.CompletedTask;
}
