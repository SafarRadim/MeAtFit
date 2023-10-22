using CommunityToolkit.Mvvm.Messaging;

namespace project.App.Services;

public interface IMessengerService
{
    IMessenger Messenger { get; }

    void Send<TMessage>(TMessage message)
        where TMessage : class;
}