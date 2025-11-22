#include "Event_info.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iomanip>

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

//Determines if event is all-day
bool Event_info::is_all_day() const {
    return start_time == "" || start_time == "N/A";
}

//Convert into ISO-8601 timestamp (YYYY-MM-DDTHH:MM:SS)
//FullCalendar only understands ISO format
static string convert_to_iso(const string& mmddyy, const string& time) {

    //Extract "MM", "DD", and "YY" from MMDDYY
    string MM = mmddyy.substr(0, 2);   // first 2 chars = month
    string DD = mmddyy.substr(2, 2);   // next 2 chars = day
    string YY = "20" + mmddyy.substr(4, 2); // last 2 chars = year

    //If no time exists (all-day event), return just YYYY-MM-DD
    if (time == "" || time == "N/A")
        return YY + "-" + MM + "-" + DD;

    //Variables to store parsed time
    int hour, minute;
    string ampm;
    char colon;

    //Parse time
    stringstream ss(time);
    ss >> hour >> colon >> minute >> ampm;

    //Convert to 24-hour time
    if (ampm == "PM" && hour != 12)
        hour += 12;    

    if (ampm == "AM" && hour == 12)
        hour = 0;      

    //Build final ISO format string
    stringstream out;
    out << YY << "-" << MM << "-" << DD << "T"
        << setw(2) << setfill('0') << hour << ":"   // HH
        << setw(2) << setfill('0') << minute << ":00"; // MM:SS

    return out.str();  //Example: "2025-10-02T15:30:00"
}
//Makes string safe for JSON by escaping quotes and backslashes
static string escape_json(const string& s) {
    string out = "";
    for (char c : s) {
        if (c == '\"')
            out += "\\\"";
        else if (c == '\\')
            out += "\\\\";
        else
            out += c;
    }
    return out;
}

//JSON output
string Event_info::to_json() const {
    string iso_start = convert_to_iso(date, start_time);
    string iso_end = convert_to_iso(date, end_time);

    string json = "{ ";
    json += "\"id\": \"" + escape_json(id) + "\", ";
    json += "\"title\": \"" + escape_json(title) + "\", ";

    if (is_all_day()) {
        json += "\"start\": \"" + iso_start + "\", ";
        json += "\"allDay\": true, ";
    }
    else {
        json += "\"start\": \"" + iso_start + "\", ";
        json += "\"end\": \"" + iso_end + "\", ";
        json += "\"allDay\": false, ";
    }

    if (location == "N/A" || location == "")
        json += "\"location\": null, ";
    else
        json += "\"location\": \"" + escape_json(location) + "\", ";

    json += "\"tag\": \"" + escape_json(tag) + "\" }";

    return json;
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
