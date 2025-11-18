#pragma once
#include <fstream> // Required for file stream operations
#include <iostream> // Required for input/output operations like cout
#include <string> 
#include <unordered_map>
#include <sstream>
#include <queue>
#include "Event_info.h"

struct Compare_Event_info{
    bool operator()(const Event_info* a, const Event_info* b) const
    {
        return *a < *b;
    }
};

int main(){
    string line;
    unordered_map<string, Event_info*> event_map;
    ifstream events_file("events.txt");
    priority_queue<Event_info*, vector<Event_info*>, Compare_Event_info> pq;
    //Error quit program
    if(!events_file.is_open()){
        return 0; 
    }
    string title;
    string date; 
    string start_time;
    string end_time;
    string tag;
    int count = 0;
    string id;
    //iterate through file
    while(getline(events_file, line)){
        istringstream event_line(line);
        string segment;
        while(getline(event_line, segment, '|')){
            count ++;
            if(count == 1){
                title = segment;
            }
            else if (count == 2){
                date = segment.substr(1, 2) + segment.substr(4, 2) + segment.substr(9, 2);
                start_time = segment.substr(12, 8);
            }
            else if(count == 3){
                end_time = segment.substr(12, 8);
            }
            else{
                tag = segment;
            }
            if(count == 4){
                id = date + '0';
                int id_count = 0;
                while(event_map.find(id) != event_map.end()){
                    id_count++;
                    id = date + to_string(id_count);
                }
                event_map[id] = new Event_info(id, title, date, start_time, end_time, "", tag);
                //Reset variables
                title = "";
                date = "";
                start_time = "";
                end_time = "";
                count = 0;
                id = "";
            }
        }
    }

    events_file.close();

    for(const auto& pair : event_map){
        pq.push(pair.second);
    }

    while(!pq.empty()){
        //cout << pq.top()->get_date() << " " << pq.top()->get_start() << " " << pq.top()->get_title() << endl;
        pq.top()->print();
        pq.pop();
    }

};