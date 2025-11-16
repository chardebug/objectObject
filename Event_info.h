#include <string>

using namespace std;

class Event_info{
    public:
        Event_info();
        Event_info(string t, int d, string start, string end, string l, string event_tag);
        string get_title();
        int get_date();
        string get_start();
        string get_end();
        string get_location();
        string get_tag();
    private:
        string title;
        int date;
        string start_time;
        string end_time;
        string location;
        string tag;

};