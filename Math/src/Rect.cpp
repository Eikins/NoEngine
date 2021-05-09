#include "Math/Rect.h"

namespace Math
{
    std::ostream& operator<<(std::ostream& out, const Rect& rect) { return out << "Rect(" << rect.position << ", " << rect.size << ")"; }
}