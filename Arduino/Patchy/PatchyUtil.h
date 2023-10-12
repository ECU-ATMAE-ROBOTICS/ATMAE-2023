// PatchyUtil.h
#ifndef PATCHYUTIL_H
#define PATCHYUTIL_H

class PatchyUtil {
public:
    /**
     * @brief Calculate a hash value for the input string using the djb2 algorithm.
     *
     * The djb2 algorithm computes a hash value for the input string.
     *
     * @param str The input string for which the hash is calculated.
     * @return The calculated hash value as a long integer.
     */
    static long hashString(const String str);
};

#endif
