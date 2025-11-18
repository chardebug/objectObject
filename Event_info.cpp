#include "Event_info.h"
#include <iostream>

Event_info::Event_info()
{
    //id = date in month/day/year with last digit 0 if only event on that date, 1 if second etc
    id = "0000000";
    title = "";
    date = "000000";
    start_time = "";
    end_time = "";
    location = "";
    tag = "";
}

Event_info::Event_info(string event_id, string t, string d, string start, string end, string l, string event_tag)
{
    id = event_id;
    title = t;
    date = d;
    start_time = start;
    end_time = end;
    location = l;
    tag = event_tag;
}

string Event_info::get_id()
{
    return id;
}

string Event_info::get_title()
{
    return title;
}

string Event_info::get_date()
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

void Event_info::print()
{
    cout << id << " " << title << " " << date << " " << start_time << " " << end_time << " " << location << " " << tag << endl;
    return;
}

bool Event_info::operator<(const Event_info& other) const
{
    string a_year = this->date.substr(4, 2);
    string b_year = other.date.substr(4, 2);
    string a_mon = this->date.substr(0, 2);
    string b_mon = other.date.substr(0, 2);
    string a_day = this->date.substr(2, 2);
    string b_day = other.date.substr(2, 2);
    if(this->date == other.date){
        if(this->start_time == other.start_time){
            return this->title > other.title;
        }
        else{
            return this->start_time > other.start_time;
        }

    }
    else {
        if(a_year == b_year){
            if(a_mon == b_mon){
                return a_day > b_day;
            }
            else{
                return a_mon > b_mon;
            }
        }
        else{
            return a_year > b_year;
        }
    }
}
