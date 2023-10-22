using System.Collections.ObjectModel;

namespace project.BL.Models;

public record TagDetailModel : ModelBase
{
    public required string TagName { get; set; }
    public ObservableCollection<ActivityListModel> Activites { get; init; } = new();
    public static TagDetailModel Empty => new()
    {
        Id = Guid.NewGuid(),
        TagName = string.Empty,
    };
}
