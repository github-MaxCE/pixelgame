#pragma once
#include "olcPixelGameEngine.h"

namespace max
{
    extern bool inRange(int low, int high, int x);

    template<class T1 = int32_t, class T2 = uint32_t>
    bool PointInsideRect(const olc::v2d_generic<T1>& point, const olc::v2d_generic<T1>& rectPos, const olc::v2d_generic<T2>& rectSize)
    {
        return (point.x >= rectPos.x) &&
               (point.y >= rectPos.y) &&
               (point.x <= rectPos.x + rectSize.x) &&
               (point.y <= rectPos.y + rectSize.y);
    }

    template<class T1 = int32_t, class T2 = uint32_t>
    bool RectInsideRect(const olc::v2d_generic<T1>& r1p, const olc::v2d_generic<T2>& r1sz, const olc::v2d_generic<T1>& r2p, const olc::v2d_generic<T2>& r2sz)
    {
        // To check if either rectangle is actually a line
        // For example :  l1 ={-1,0}  r1={1,1}  l2={0,-1}
        // r2={0,1}


        auto r1 = r1p + r1sz;
        auto r2 = r2p + r2sz;

        // If one rectangle is on left side of other
        if (r1.x <= r2p.x || r2.x <= r1p.x
            ||
            r1.y <= r2p.y || r2.y <= r1p.y) return false;

        return true;
    }
}