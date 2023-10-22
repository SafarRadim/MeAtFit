namespace project.BL.Models;

public record TagActivityListModel : ModelBase
{
    public required Guid TagId { get; set; }
    public required string TagName { get; set; }

    public static TagActivityListModel Empty => new()
    {
        Id = Guid.NewGuid(),
        TagId = Guid.Empty,
        TagName = string.Empty,
    };
}
