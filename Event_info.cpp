#include "Event_info.h"

Event_info::Event_info()
{
    title = "";
    //Date = month/day/year
    date = 000000;
    start_time = "";
    end_time = "";
    location = "";
    tag = "";
}

Event_info::Event_info(string t, int d, string start, string end, string l, string event_tag)
{
    title = t;
    date = d;
    start_time = start;
    end_time = end;
    location = l;
    tag = event_tag;
}

string Event_info::get_title()
{
    return title;
}

int Event_info::get_date()
{
    return date;
}

string Event_info::get_start()
{
    return start_time;
}

string Event_info::get_end()
{
    return end_time;
}

string Event_info::get_location()
{
    return location;
}

string Event_info::get_tag()
{
    return tag;
}
