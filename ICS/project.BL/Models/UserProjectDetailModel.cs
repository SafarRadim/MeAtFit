using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace project.BL.Models;

public record UserProjectDetailModel : ModelBase
{
    public required Guid UserId { get; set; }
    public required string UserName { get; set; }

    public static UserProjectDetailModel Empty => new()
    {
        Id = Guid.NewGuid(),
        UserId = Guid.Empty,
        UserName = string.Empty,
    };
}
