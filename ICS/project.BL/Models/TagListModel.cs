using project.BL.Models;

public record TagListModel : ModelBase
{
    public required string TagName { get; set; }
    public static TagListModel Empty => new()
    {
        Id = Guid.NewGuid(),
        TagName = string.Empty,
    };
}
