#include "cell.hxx"

Cell::Cell(Point isoCoordinates) : _isoCoordinates(isoCoordinates), _tileID(14)
{
  _sprite = std::shared_ptr<Sprite>(new Sprite(_tileID, _isoCoordinates));
}

void Cell::increaseHeightOfCell()
{
  int height = _isoCoordinates.getHeight();

  if ( height < _maxCellHeight )
  {
    _isoCoordinates.setHeight(_isoCoordinates.getHeight() + 1);
    _sprite->setTileIsoCoordinates(_isoCoordinates);
  }
}

void Cell::decreaseHeightOfCell()
{
  int height = _isoCoordinates.getHeight();
  
  if ( height > 0 )
  {
    _isoCoordinates.setHeight(height - 1);
    _sprite->setTileIsoCoordinates(_isoCoordinates);
  }
}
