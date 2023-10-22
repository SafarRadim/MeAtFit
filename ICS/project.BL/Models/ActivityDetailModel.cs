using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace project.BL.Models;

public record ActivityDetailModel : ModelBase
{
    public required string Description { get; set; }
    public required string Name { get; set; }
    public required DateTime Start { get; set; }
    public DateTime End { get; set; }
    public Guid? ProjectId { get; set; }
    public Guid UserId { get; set; }
    public ObservableCollection<TagActivityListModel> TagActivities { get; init; } = new();
    public static ActivityDetailModel Empty => new()
    {
        Id = Guid.NewGuid(),
        ProjectId = Guid.Empty,
        UserId = Guid.Empty,
        Description = string.Empty,
        Name = string.Empty,
        Start = DateTime.Today,
        End = DateTime.Today
    };
}
