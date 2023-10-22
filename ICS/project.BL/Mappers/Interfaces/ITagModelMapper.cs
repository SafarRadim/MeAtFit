using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public interface ITagModelMapper : IModelMapper<TagEntity, TagListModel, TagDetailModel>
{
}