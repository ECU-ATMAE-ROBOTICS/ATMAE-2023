// PatchyUtil.h
#ifndef PATCHYUTIL_H
#define PATCHYUTIL_H

class PatchyUtil
{
public:
    /**
     * @brief Calculate a hash value for the input string.
     *
     * @param str The input string for which the hash is calculated.
     * @return The calculated hash value as a long integer.
     */
    static long hashString(const String &str);

    /**
     * Enum defining different directions as hashes.
     */
    enum class Instruction : long
    {
        Up = 5863882,
        Down = 2090192221,
        Left = 2090468272,
        Right = 273236323,
        Grab = 2090302561
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

    /**
     * Enum defining different axes.
     */
    enum class Axis
    {
        X,
        Y
    };
};

#endif
