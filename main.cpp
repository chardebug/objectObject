/*
    FIX EXPLANATION:

      - Reset "count" for every new event so every line starts at field #1 again.
      - Trim whitespace so fields are clean.
      - Extract the date using known fixed positions (MM/DD/YYYY is always 10 chars).
      - Extract the time by checking if there is anything AFTER the date,
        instead of assuming an exact character location.

    Result: fields no longer bleed into each other, dates/times parse correctly,
            and the JSON output becomes normalized.
*/

#include <fstream> 
#include <iostream> 
#include <string> 
#include <unordered_map>
#include <sstream>
#include <queue>
#include "Event_info.h"

//Comparison for pq
struct Compare_Event_info {
    bool operator()(const Event_info* a, const Event_info* b) const
    {
        return *a < *b;
    }
};

//Trim whitespace
static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

int main() {
    //Iniliation varibles
    string line;
    string title;
    string date;
    string start_time;
    string end_time;
    string tag;
    int count = 0;
    string id;
    string location;
    string segment;
    unordered_map<string, Event_info*> event_map;
    ifstream events_file("events.txt");
    priority_queue<Event_info*, vector<Event_info*>, Compare_Event_info> pq;

    //Error quit program
    if (!events_file.is_open()) {
        return 0;
    }

    //iterate through file
    while (getline(events_file, line)) {
        //Reset counter at start of every event so it doesn't carry over when parsing
        count = 0; 
        istringstream event_line(line);
        while (getline(event_line, segment, '|')) {
            //Always trim
            segment = trim(segment); 
            count++;
            //Title
            if (count == 1) {
                title = segment;
            }
            //Date + Start time
            else if (count == 2) {
                // Parse date + optional time: 
                // First 10 chars = MM/DD/YYYY
                // Anything after that = start time
                // If only the date is present, then all-day event
                if (segment.length() >= 10) {
                    string MM = segment.substr(0, 2);
                    string DD = segment.substr(3, 2);
                    string YY = segment.substr(8, 2);
                    date = MM + DD + YY;
                }
                else {
                    date = "000000";
                }
                
                // If over 10 char date, then start time
                if (segment.length() > 10) {
                    start_time = trim(segment.substr(10));
                }
                else {
                    start_time = "N/A";
                }
            }

            //End time
            // Parse date + optional time
            // Same format as start time
            // If text exists after 10-char date, then it is the end time
            // Otherwise, it's an all-day event
            else if (count == 3) {
                if (segment.length() > 10) {
                    end_time = trim(segment.substr(10));
                }
                else {
                    end_time = "N/A";
                }
            }

            //Location
            else if (count == 4) {
                location = segment;
                if (location == "") location = "N/A";
            }
            //Tag
            else {
                tag = segment;
            }
            //Add to map and reset variables for next line
            if (count == 5) {
                id = date + '0';
                int id_count = 0;
                while (event_map.find(id) != event_map.end()) {
                    id_count++;
                    id = date + to_string(id_count);
                }
                event_map[id] = new Event_info(id, title, date, start_time, end_time, location, tag);
                //Reset variables
                title = "";
                date = "";
                start_time = "";
                end_time = "";
                count = 0;
                id = "";
                location = "";
            }
        }
    }

    events_file.close();
    //Add event_info objects to priority queue
    for (const auto& pair : event_map) {
        pq.push(pair.second);
    }

    //Write JSON by popping events from the priority queue in sorted order
    ofstream json("events.json");
    json << "[\n";

    bool first = true;
    while (!pq.empty()) {
        // Get the highest-priority event (earliest date/time) from the priority queue
        Event_info* ev = pq.top();
        pq.pop();
        // Add a comma before all items except the first
        if (!first) json << ",\n"; 
        first = false;
        //Write event as JSON object
        json << "  " << ev->to_json();
    }

    json << "\n]\n";
    json.close();

    cout << "events.json created.\n";

    return 0;
}
