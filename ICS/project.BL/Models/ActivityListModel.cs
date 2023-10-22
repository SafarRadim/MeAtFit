using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace project.BL.Models;

public record ActivityListModel : ModelBase
{
    public required string Name { get; set; }
    public DateTime End { get; set; }
    public ObservableCollection<TagListModel> Tags { get; init; } = new();

    public static ActivityListModel Empty => new()
    {
        Id = Guid.NewGuid(),
        Name = string.Empty,
        End = DateTime.MaxValue
    };
}
