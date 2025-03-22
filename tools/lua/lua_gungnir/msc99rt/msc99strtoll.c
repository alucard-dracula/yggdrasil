// msc99strtoll.cpp

#include <mscompatibility/warning_check.h>

#if defined(_MSC_VER) && (_MSC_VER < 1800)

#include <mscompatibility/strtoll.h>

#include <ctype.h>
#include <errno.h>

#define ISSPACE(x) isspace((x))
#define ISDIGIT(x) isdigit((x))
#define ISALPHA(x) isalpha((x))
#define ISUPPER(x) isupper((x))

#ifndef ULLONG_MAX
#	define ULLONG_MAX (~(uint64_t)0) /* 0xFFFFFFFFFFFFFFFF */
#endif // ULLONG_MAX

#ifndef LLONG_MAX
#	define LLONG_MAX ((int64_t)(ULLONG_MAX >> 1)) /* 0x7FFFFFFFFFFFFFFF */
#endif // LLONG_MAX

#ifndef LLONG_MIN
#	define LLONG_MIN (~LLONG_MAX) /* 0x8000000000000000 */
#endif // LLONG_MIN

int64_t strtoll(const char *nptr, char **endptr, register int base)
{
        register const char *s = nptr;
        register uint64_t acc;
        register int c;
        register uint64_t cutoff;
        register int neg = 0, any, cutlim;
        /*
         * Skip white space and pick up leading +/- sign if any.
         * If base is 0, allow 0x for hex and 0 for octal, else
         * assume decimal; if base is already 16, allow 0x.
         */
        do {
                c = *s++;
        } while (ISSPACE(c));
        if (c == '-') {
                neg = 1;
                c = *s++;
        } else if (c == '+')
                c = *s++;
        if ((base == 0 || base == 16) &&
            c == '0' && (*s == 'x' || *s == 'X')) {
                c = s[1];
                s += 2;
                base = 16;
        }
        if (base == 0)
                base = c == '0' ? 8 : 10;
        /*
         * Compute the cutoff value between legal numbers and illegal
         * numbers.  That is the largest legal value, divided by the
         * base.  An input number that is greater than this value, if
         * followed by a legal input character, is too big.  One that
         * is equal to this value may be valid or not; the limit
         * between valid and invalid numbers is then based on the last
         * digit.  For instance, if the range for longs is
         * [-2147483648..2147483647] and the input base is 10,
         * cutoff will be set to 214748364 and cutlim to either
         * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
         * a value > 214748364, or equal but the next digit is > 7 (or 8),
         * the number is too big, and we will return a range error.
         *
         * Set any if any `digits' consumed; make it negative to indicate
         * overflow.
         */
        cutoff = neg ? -(uint64_t)LLONG_MIN : LLONG_MAX;
        cutlim = cutoff % (uint64_t)base;
        cutoff /= (uint64_t)base;
        for (acc = 0, any = 0;; c = *s++) {
                if (ISDIGIT(c))
                        c -= '0';
                else if (ISALPHA(c))
                        c -= ISUPPER(c) ? 'A' - 10 : 'a' - 10;
                else
                        break;
                if (c >= base)
                        break;
                if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
                        any = -1;
                else {
                        any = 1;
                        acc *= base;
                        acc += c;
                }
        }
        if (any < 0) {
                acc = neg ? LLONG_MIN : LLONG_MAX;
                errno = ERANGE;
        } else if (neg)
                acc = -acc;
        if (endptr != 0)
                *endptr = (char *) (any ? s - 1 : nptr);
        return (acc);
}

uint64_t strtoull(const char *nptr, char **endptr, register int base)
{
        register const char *s = nptr;
        register uint64_t acc;
        register int c;
        register uint64_t cutoff;
        register int neg = 0, any, cutlim;
        /*
         * See strtol for comments as to the logic used.
         */
        do {
                c = *s++;
        } while (ISSPACE(c));
        if (c == '-') {
                neg = 1;
                c = *s++;
        } else if (c == '+')
                c = *s++;
        if ((base == 0 || base == 16) &&
            c == '0' && (*s == 'x' || *s == 'X')) {
                c = s[1];
                s += 2;
                base = 16;
        }
        if (base == 0)
                base = c == '0' ? 8 : 10;
        cutoff = (uint64_t)ULLONG_MAX / (uint64_t)base;
        cutlim = (uint64_t)ULLONG_MAX % (uint64_t)base;
        for (acc = 0, any = 0;; c = *s++) {
                if (ISDIGIT(c))
                        c -= '0';
                else if (ISALPHA(c))
                        c -= ISUPPER(c) ? 'A' - 10 : 'a' - 10;
                else
                        break;
                if (c >= base)
                        break;
                if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
                        any = -1;
                else {
                        any = 1;
                        acc *= base;
                        acc += c;
                }
        }
        if (any < 0) {
                acc = ULLONG_MAX;
                errno = ERANGE;
        } else if (neg)
                acc = -acc;
        if (endptr != 0)
                *endptr = (char *) (any ? s - 1 : nptr);
        return (acc);
}

#endif // _MSC_VER