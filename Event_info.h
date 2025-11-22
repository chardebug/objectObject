#include <string>

using namespace std;

class Event_info{
    public:
        Event_info();
        Event_info(string event_id, string t, string d, string start, string end, string l, string event_tag);
        string get_id();
        string get_title();
        string get_date();
        string get_start();
        string get_end();
        string get_location();
        string get_tag();
        void print();
        bool operator<(const Event_info& other) const;
        bool is_all_day() const;
        string to_json() const;
    private:
        string id;
        string title;
        string date;
        string start_time;
        string end_time;
        string location;
        string tag;

};
