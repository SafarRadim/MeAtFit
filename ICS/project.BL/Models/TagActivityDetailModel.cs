namespace project.BL.Models;

public record TagActivityDetailModel : ModelBase
{
    public required Guid TagId { get; set; }
    public required string TagName { get; set; }

    public static TagActivityDetailModel Empty => new()
    {
        Id = Guid.NewGuid(),
        TagId = Guid.Empty,
        TagName = string.Empty,
    };
}
