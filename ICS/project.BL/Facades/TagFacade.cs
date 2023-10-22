using project.BL.Mappers;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.UnitOfWork;

namespace project.BL.Facades;

public class TagFacade : FacadeBase<TagEntity, TagListModel, TagDetailModel, TagEntityMapper>,
    ITagFacade
{
    public TagFacade(
        IUnitOfWorkFactory unitOfWorkFactory,
        ITagModelMapper modelMapper)
        : base(unitOfWorkFactory, modelMapper)
    {
    }
}