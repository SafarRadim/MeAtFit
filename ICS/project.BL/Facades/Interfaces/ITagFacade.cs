using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Facades;

public interface ITagFacade : IFacade<TagEntity, TagListModel, TagDetailModel>
{
}