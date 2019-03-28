#include "intf/file/Output.h"

using namespace intf::file;

typename Output::PositionType Output::SeekPosition(const PositionType & pos)
{
    return SeekPosition(pos, (ModeValueType)ModeType::output);
}

typename Output::PositionType Output::SeekOffset(const OffsetType & off,
    const WayType & way)
{
    return SeekOffset(off, way, (ModeValueType)ModeType::output);
}
