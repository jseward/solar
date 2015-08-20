#include "strptime.h"
#include <string>

//http://trac.rtmpd.com/browser/trunk/sources/common/src/platform/windows/strptime.cpp

static const char *abb_weekdays[] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	nullptr
};

static const char *full_weekdays[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",
	nullptr
};

static const char *abb_month[] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec",
	nullptr
};

static const char *full_month[] = {
	"January",
	"February",
	"Mars",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December",
	nullptr,
};

static const char *ampm[] = {
	"am",
	"pm",
	nullptr
};

/*
 * Try to match `*buf' to one of the strings in `strs'.  Return the
 * index of the matching string (or -1 if none).  Also advance buf.
 */

static int
match_string(const char **buf, const char **strs) {
	int i = 0;
	for (i = 0; strs[i] != nullptr; ++i) {
		int len = (int) strlen(strs[i]);
		if (_strnicmp(*buf, strs[i], len) == 0) {
			*buf += len;
			return i;
		}
	}
	return -1;
}

/*
 * tm_year is relative this year */

const int tm_year_base = 1900;

/*
 * Return TRUE iff `year' was a leap year.
 */

static int
is_leap_year(int year) {
	return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}

/*
 * Return the weekday [0,6] (0 = Sunday) of the first day of `year'
 */

static int
first_day(int year) {
	int ret = 4;

	for (; year > 1970; --year)
		ret = (ret + 365 + is_leap_year(year) ? 1 : 0) % 7;
	return ret;
}

/*
 * Set `timeptr' given `wnum' (week number [0, 53])
 */

static void
set_week_number_sun(struct tm *timeptr, int wnum) {
	int fday = first_day(timeptr->tm_year + tm_year_base);

	timeptr->tm_yday = wnum * 7 + timeptr->tm_wday - fday;
	if (timeptr->tm_yday < 0) {
		timeptr->tm_wday = fday;
		timeptr->tm_yday = 0;
	}
}

/*
 * Set `timeptr' given `wnum' (week number [0, 53])
 */

static void
set_week_number_mon(struct tm *timeptr, int wnum) {
	int fday = (first_day(timeptr->tm_year + tm_year_base) + 6) % 7;

	timeptr->tm_yday = wnum * 7 + (timeptr->tm_wday + 6) % 7 - fday;
	if (timeptr->tm_yday < 0) {
		timeptr->tm_wday = (fday + 1) % 7;
		timeptr->tm_yday = 0;
	}
}

/*
 * Set `timeptr' given `wnum' (week number [0, 53])
 */

static void
set_week_number_mon4(struct tm *timeptr, int wnum) {
	int fday = (first_day(timeptr->tm_year + tm_year_base) + 6) % 7;
	int offset = 0;

	if (fday < 4)
		offset += 7;

	timeptr->tm_yday = offset + (wnum - 1) * 7 + timeptr->tm_wday - fday;
	if (timeptr->tm_yday < 0) {
		timeptr->tm_wday = fday;
		timeptr->tm_yday = 0;
	}
}

/*
 *
 */

char *
strptime(const char *buf, const char *fmt, struct tm *timeptr) {
	char c;

	for (; (c = *fmt) != '\0'; ++fmt) {
		char *s;
		int ret;

		if (isspace(c)) {
			while (isspace(*buf))
				++buf;
		} else if (c == '%' && fmt[1] != '\0') {
			c = *++fmt;
			if (c == 'E' || c == 'O')
				c = *++fmt;
			switch (c) {
				case 'A':
					ret = match_string(&buf, full_weekdays);
					if (ret < 0)
						return nullptr;
					timeptr->tm_wday = ret;
					break;
				case 'a':
					ret = match_string(&buf, abb_weekdays);
					if (ret < 0)
						return nullptr;
					timeptr->tm_wday = ret;
					break;
				case 'B':
					ret = match_string(&buf, full_month);
					if (ret < 0)
						return nullptr;
					timeptr->tm_mon = ret;
					break;
				case 'b':
				case 'h':
					ret = match_string(&buf, abb_month);
					if (ret < 0)
						return nullptr;
					timeptr->tm_mon = ret;
					break;
				case 'C':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_year = (ret * 100) - tm_year_base;
					buf = s;
					break;
				case 'c':
					abort();
				case 'D': /* %m/%d/%y */
					s = strptime(buf, "%m/%d/%y", timeptr);
					if (s == nullptr)
						return nullptr;
					buf = s;
					break;
				case 'd':
				case 'e':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_mday = ret;
					buf = s;
					break;
				case 'H':
				case 'k':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_hour = ret;
					buf = s;
					break;
				case 'I':
				case 'l':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					if (ret == 12)
						timeptr->tm_hour = 0;
					else
						timeptr->tm_hour = ret;
					buf = s;
					break;
				case 'j':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_yday = ret - 1;
					buf = s;
					break;
				case 'm':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_mon = ret - 1;
					buf = s;
					break;
				case 'M':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_min = ret;
					buf = s;
					break;
				case 'n':
					if (*buf == '\n')
						++buf;
					else
						return nullptr;
					break;
				case 'p':
					ret = match_string(&buf, ampm);
					if (ret < 0)
						return nullptr;
					if (timeptr->tm_hour == 0) {
						if (ret == 1)
							timeptr->tm_hour = 12;
					} else
						timeptr->tm_hour += 12;
					break;
				case 'r': /* %I:%M:%S %p */
					s = strptime(buf, "%I:%M:%S %p", timeptr);
					if (s == nullptr)
						return nullptr;
					buf = s;
					break;
				case 'R': /* %H:%M */
					s = strptime(buf, "%H:%M", timeptr);
					if (s == nullptr)
						return nullptr;
					buf = s;
					break;
				case 'S':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_sec = ret;
					buf = s;
					break;
				case 't':
					if (*buf == '\t')
						++buf;
					else
						return nullptr;
					break;
				case 'T': /* %H:%M:%S */
				case 'X':
					s = strptime(buf, "%H:%M:%S", timeptr);
					if (s == nullptr)
						return nullptr;
					buf = s;
					break;
				case 'u':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_wday = ret - 1;
					buf = s;
					break;
				case 'w':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_wday = ret;
					buf = s;
					break;
				case 'U':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					set_week_number_sun(timeptr, ret);
					buf = s;
					break;
				case 'V':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					set_week_number_mon4(timeptr, ret);
					buf = s;
					break;
				case 'W':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					set_week_number_mon(timeptr, ret);
					buf = s;
					break;
				case 'x':
					s = strptime(buf, "%Y:%m:%d", timeptr);
					if (s == nullptr)
						return nullptr;
					buf = s;
					break;
				case 'y':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					if (ret < 70)
						timeptr->tm_year = 100 + ret;
					else
						timeptr->tm_year = ret;
					buf = s;
					break;
				case 'Y':
					ret = strtol(buf, &s, 10);
					if (s == buf)
						return nullptr;
					timeptr->tm_year = ret - tm_year_base;
					buf = s;
					break;
				case 'Z':
					abort();
				case '\0':
					--fmt;
					/* FALLTHROUGH */
				case '%':
					if (*buf == '%')
						++buf;
					else
						return nullptr;
					break;
				default:
					if (*buf == '%' || *++buf == c)
						++buf;
					else
						return nullptr;
					break;
			}
		} else {
			if (*buf == c)
				++buf;
			else
				return nullptr;
		}
	}
	return (char *) buf;
}
