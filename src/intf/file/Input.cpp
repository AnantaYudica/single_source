#include "intf/file/Input.h"

using namespace intf::file;

typename Input::PositionType Input::SeekPosition(const PositionType & pos)
{
    return SeekPosition(pos, (ModeValueType)ModeType::input);
}

typename Input::PositionType Input::SeekOffset(const OffsetType & off,
    const WayType & way)
{
    return SeekOffset(off, way, (ModeValueType)ModeType::input);
}
