/*
*******************************************************************************
*
*   Copyright (C) 2009, International Business Machines
*   Corporation and others.  All Rights Reserved.
*
*******************************************************************************
*   file name:  normalizer2.h
*   encoding:   US-ASCII
*   tab size:   8 (not used)
*   indentation:4
*
*   created on: 2009nov22
*   created by: Markus W. Scherer
*/

#ifndef __NORMALIZER2_H__
#define __NORMALIZER2_H__

/**
 * \file
 * \brief C++ API: New API for Unicode Normalization.
 */

#include "unicode/utypes.h"

#if !UCONFIG_NO_NORMALIZATION

#include "unicode/uniset.h"
#include "unicode/unistr.h"
#include "unicode/unorm2.h"

U_NAMESPACE_BEGIN

/**
 * Unicode normalization functionality for standard Unicode normalization or
 * for using custom mapping tables.
 * All instances of this class are unmodifiable/immutable.
 * Instances returned by getInstance() are singletons that must not be deleted by the caller.
 *
 * The spanQuickCheckYes() stops at a normalization boundary.
 * At such a boundary, the portions of the string
 * before it and after it do not interact and can be handled independently.
 *
 * The set of normalization boundaries returned may not be
 * complete: There may be more boundaries that could be returned.
 * @draft ICU 4.4
 */
class U_COMMON_API Normalizer2 : public UObject {
public:
    /**
     * Returns a Normalizer2 instance which uses the specified data file
     * (packageName/name similar to ucnv_openPackage() and ures_open()/ResourceBundle)
     * and which composes or decomposes text according to the specified mode.
     * Returns an unmodifiable singleton instance. Do not delete it.
     *
     * Use packageName=NULL for data files that are part of ICU's own data.
     * Use name="nfc" and UNORM2_COMPOSE/UNORM2_DECOMPOSE for Unicode standard NFC/NFD.
     * Use name="nfkc" and UNORM2_COMPOSE/UNORM2_DECOMPOSE for Unicode standard NFKC/NFKD.
     * Use name="nfkc_cf" and UNORM2_COMPOSE for Unicode standard NFKC_CF=NFKC_Casefold.
     *
     * @param packageName NULL for ICU built-in data, otherwise application data package name
     * @param name "nfc" or "nfkc" or "nfkc_cf" or name of custom data file
     * @param mode normalization mode (compose or decompose etc.)
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return the requested Normalizer2, if successful
     * @draft ICU 4.4
     */
    static const Normalizer2 *
    getInstance(const char *packageName,
                const char *name,
                UNormalization2Mode mode,
                UErrorCode &errorCode);

    /**
     * Returns the normalized form of the source string.
     * @param src source string
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return normalized src
     * @draft ICU 4.4
     */
    UnicodeString
    normalize(const UnicodeString &src, UErrorCode &errorCode) const {
        UnicodeString result;
        normalize(src, result, errorCode);
        return result;
    }
    /**
     * Writes the normalized form of the source string to the destination string
     * (replacing its contents) and returns the destination string.
     * The source and destination strings must be different objects.
     * @param src source string
     * @param dest destination string; its contents is replaced with normalized src
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return dest
     * @draft ICU 4.4
     */
    virtual UnicodeString &
    normalize(const UnicodeString &src,
              UnicodeString &dest,
              UErrorCode &errorCode) const = 0;
    /**
     * Appends the normalized form of the second string to the first string
     * (merging them at the boundary) and returns the first string.
     * The result is normalized if the first string was normalized.
     * The first and second strings must be different objects.
     * @param first string, should be normalized
     * @param second string, will be normalized
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return first
     * @draft ICU 4.4
     */
    virtual UnicodeString &
    normalizeSecondAndAppend(UnicodeString &first,
                             const UnicodeString &second,
                             UErrorCode &errorCode) const = 0;
    /**
     * Appends the second string to the first string
     * (merging them at the boundary) and returns the first string.
     * The result is normalized if both the strings were normalized.
     * The first and second strings must be different objects.
     * @param first string, should be normalized
     * @param second string, should be normalized
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return first
     * @draft ICU 4.4
     */
    virtual UnicodeString &
    append(UnicodeString &first,
           const UnicodeString &second,
           UErrorCode &errorCode) const = 0;

    /**
     * Tests if the string is normalized.
     * Internally, in cases where the quickCheck() method would return "maybe"
     * (which is only possible for the two COMPOSE modes) this method
     * resolves to "yes" or "no" to provide a definitive result,
     * at the cost of doing more work in those cases.
     * @param s input string
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return TRUE if s is normalized
     * @draft ICU 4.4
     */
    virtual UBool
    isNormalized(const UnicodeString &s, UErrorCode &errorCode) const = 0;

    /**
     * Tests if the string is normalized.
     * For the two COMPOSE modes, the result could be "maybe" in cases that
     * would take a little more work to resolve definitively.
     * Use spanQuickCheckYes() and normalizeSecondAndAppend() for a faster
     * combination of quick check + normalization, to avoid
     * re-checking the "yes" prefix.
     * @param s input string
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return UNormalizationCheckResult
     * @draft ICU 4.4
     */
    virtual UNormalizationCheckResult
    quickCheck(const UnicodeString &s, UErrorCode &errorCode) const = 0;

    /**
     * Returns the end of the normalized substring of the input string.
     * In other words, with <code>end=spanQuickCheckYes(s, ec);</code>
     * the substring <code>UnicodeString(s, 0, end)</code>
     * will pass the quick check with a "yes" result.
     *
     * The returned end index is usually one or more characters before the
     * "no" or "maybe" character: The end index is at a normalization boundary.
     * (See the class documentation for more about normalization boundaries.)
     *
     * When the goal is a normalized string and most input strings are expected
     * to be normalized already, then call this method,
     * and if it returns a prefix shorter than the input string,
     * copy that prefix and use normalizeSecondAndAppend() for the remainder.
     * @param s input string
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return UNormalizationCheckResult
     * @draft ICU 4.4
     */
    virtual int32_t
    spanQuickCheckYes(const UnicodeString &s, UErrorCode &errorCode) const = 0;

    /**
     * ICU "poor man's RTTI", returns a UClassID for this class.
     * @returns a UClassID for this class.
     * @draft ICU 4.4
     */
    static UClassID U_EXPORT2 getStaticClassID();

    /**
     * ICU "poor man's RTTI", returns a UClassID for the actual class.
     * @return a UClassID for the actual class.
     * @draft ICU 4.4
     */
    virtual UClassID getDynamicClassID() const;
};

/**
 * Normalization filtered by a UnicodeSet.
 * Normalizes portions of the text contained in the filter set and leaves
 * portions not contained in the filter set unchanged.
 * Filtering is done via UnicodeSet::span(..., USET_SPAN_SIMPLE).
 * Not-in-the-filter text is treated as "is normalized" and "quick check yes".
 * This class implements all of (and only) the Normalizer2 API.
 * An instance of this class is unmodifiable/immutable but is constructed and
 * must be destructed by the owner.
 * @draft ICU 4.4
 */
class U_COMMON_API FilteredNormalizer2 : public Normalizer2 {
public:
    /**
     * Constructs a filtered normalizer wrapping any Normalizer2 instance
     * and a filter set.
     * Both are aliased and must not be modified or deleted while this object
     * is used.
     * The filter set should be frozen; otherwise the performance will suffer greatly.
     * @param n2 wrapped Normalizer2 instance
     * @param filterSet UnicodeSet which determines the characters to be normalized
     * @draft ICU 4.4
     */
    FilteredNormalizer2(const Normalizer2 &n2, const UnicodeSet &filterSet) :
            norm2(n2), set(filterSet) {}

    /**
     * Writes the normalized form of the source string to the destination string
     * (replacing its contents) and returns the destination string.
     * The source and destination strings must be different objects.
     * @param src source string
     * @param dest destination string; its contents is replaced with normalized src
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return dest
     * @draft ICU 4.4
     */
    virtual UnicodeString &
    normalize(const UnicodeString &src,
              UnicodeString &dest,
              UErrorCode &errorCode) const;
    /**
     * Appends the normalized form of the second string to the first string
     * (merging them at the boundary) and returns the first string.
     * The result is normalized if the first string was normalized.
     * The first and second strings must be different objects.
     * @param first string, should be normalized
     * @param second string, will be normalized
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return first
     * @draft ICU 4.4
     */
    virtual UnicodeString &
    normalizeSecondAndAppend(UnicodeString &first,
                             const UnicodeString &second,
                             UErrorCode &errorCode) const;
    /**
     * Appends the second string to the first string
     * (merging them at the boundary) and returns the first string.
     * The result is normalized if both the strings were normalized.
     * The first and second strings must be different objects.
     * @param first string, should be normalized
     * @param second string, should be normalized
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return first
     * @draft ICU 4.4
     */
    virtual UnicodeString &
    append(UnicodeString &first,
           const UnicodeString &second,
           UErrorCode &errorCode) const;

    /**
     * Tests if the string is normalized.
     * For details see the Normalizer2 base class documentation.
     * @param s input string
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return TRUE if s is normalized
     * @draft ICU 4.4
     */
    virtual UBool
    isNormalized(const UnicodeString &s, UErrorCode &errorCode) const;
    /**
     * Tests if the string is normalized.
     * For details see the Normalizer2 base class documentation.
     * @param s input string
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return UNormalizationCheckResult
     * @draft ICU 4.4
     */
    virtual UNormalizationCheckResult
    quickCheck(const UnicodeString &s, UErrorCode &errorCode) const;
    /**
     * Returns the end of the normalized substring of the input string.
     * For details see the Normalizer2 base class documentation.
     * @param s input string
     * @param errorCode Standard ICU error code. Its input value must
     *                  pass the U_SUCCESS() test, or else the function returns
     *                  immediately. Check for U_FAILURE() on output or use with
     *                  function chaining. (See User Guide for details.)
     * @return UNormalizationCheckResult
     * @draft ICU 4.4
     */
    virtual int32_t
    spanQuickCheckYes(const UnicodeString &s, UErrorCode &errorCode) const;

    /**
     * ICU "poor man's RTTI", returns a UClassID for this class.
     * @returns a UClassID for this class.
     * @draft ICU 4.4
     */
    static UClassID U_EXPORT2 getStaticClassID();

    /**
     * ICU "poor man's RTTI", returns a UClassID for the actual class.
     * @return a UClassID for the actual class.
     * @draft ICU 4.4
     */
    virtual UClassID getDynamicClassID() const;
private:
    UnicodeString &
    normalize(const UnicodeString &src,
              UnicodeString &dest,
              USetSpanCondition spanCondition,
              UErrorCode &errorCode) const;

    UnicodeString &
    normalizeSecondAndAppend(UnicodeString &first,
                             const UnicodeString &second,
                             UBool doNormalize,
                             UErrorCode &errorCode) const;

    const Normalizer2 &norm2;
    const UnicodeSet &set;
};

U_NAMESPACE_END

#endif  // !UCONFIG_NO_NORMALIZATION
#endif  // __NORMALIZER2_H__
