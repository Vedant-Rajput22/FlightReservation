#include "core/Util.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
int hhmmToMinutes(const std::string &hhmm)
{
    int h = 0, m = 0;
    char c = ':';
    std::istringstream is(hhmm);
    is >> h >> c >> m;
    return h * 60 + m;
}
std::string minutesToHHMM(int minutes)
{
    if (minutes < 0)
        minutes = 0;
    int h = (minutes / 60) % 24;
    int m = minutes % 60;
    std::ostringstream os;
    os << std::setw(2) << std::setfill('0') << h << ":" << std::setw(2) << std::setfill('0') << m;
    return os.str();
}
static std::tm parseDate(const std::string &ymd)
{
    std::tm t{};
    int y = 0, mo = 0, d = 0;
    char dash = '-';
    std::istringstream is(ymd);
    is >> y >> dash >> mo >> dash >> d;
    t.tm_year = y - 1900;
    t.tm_mon = mo - 1;
    t.tm_mday = d;
    t.tm_hour = 12;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;
    return t;
}
int dayOfWeek(const std::string &yyyy_mm_dd)
{
    std::tm t = parseDate(yyyy_mm_dd);
    std::time_t tt = std::mktime(&t); // local time
    std::tm *lt = std::localtime(&tt);
    int w = lt->tm_wday; // 0=Sun..6=Sat
    // Return 1=Mon..7=Sun
    return w == 0 ? 7 : w;
}
bool operatesOn(const std::string &days, int dow)
{
    char c = char('0' + dow);
    return days.find(c) != std::string::npos;
}
std::string nowIsoLocal()
{
    std::time_t t = std::time(nullptr);
    std::tm *lt = std::localtime(&t);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt);
    return std::string(buf);
}

std::string trim(const std::string &s)
{
    auto b = s.begin();
    while (b != s.end() && std::isspace(static_cast<unsigned char>(*b)))
        ++b;
    auto e = s.end();
    do
    {
        if (e == b)
            break;
        --e;
    } while (std::isspace(static_cast<unsigned char>(*e)));
    return std::string(b, e + 1);
}

std::vector<std::string> splitCSVLine(const std::string &line)
{
    std::vector<std::string> out;
    std::string cur;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); ++i)
    {
        char ch = line[i];
        if (ch == '"')
        {
            inQuotes = !inQuotes;
        }
        else if (ch == ',' && !inQuotes)
        {
            out.push_back(trim(cur));
            cur.clear();
        }
        else
        {
            cur.push_back(ch);
        }
    }
    out.push_back(trim(cur));
    return out;
}