// PatchyUtil.h
#ifndef PATCHYUTIL_H
#define PATCHYUTIL_H

class PatchyUtil
{
public:
    struct Coordinate
    {
        int x;
        int y;
    };

    /**
     * Enum defining different completion statuses .
     */
    enum class Status
    {
        Unset = -1,
        Failure,
        Success,
        Invalid
    };

    enum class State 
    {
      Receive,
      Parse,
      Grab,
      Drop,
      Response,
      Reset
    };
};

#endif
