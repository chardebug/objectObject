#pragma once
#include <fstream> 
#include <iostream> 
#include <string> 
#include <unordered_map>
#include <sstream>
#include <queue>
#include "Event_info.h"

//Comparison for pq
struct Compare_Event_info{
    bool operator()(const Event_info* a, const Event_info* b) const
    {
        return *a < *b;
    }
};

int main(){
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
    if(!events_file.is_open()){
        return 0; 
    }
    //iterate through file
    while(getline(events_file, line)){
        istringstream event_line(line);
        while(getline(event_line, segment, '|')){
            count ++;
            //Title
            if(count == 1){
                title = segment;
            }
            //Date and start time
            else if (count == 2){
                date = segment.substr(1, 2) + segment.substr(4, 2) + segment.substr(9, 2);
                if(segment.length() < 20){
                    start_time = "12:00 AM";
                }
                else{
                    start_time = segment.substr(12, 8);
                }
            }
            //End time
            else if(count == 3){
                if(segment.length() < 20){
                    end_time = "11:59 PM";
                }
                else{
                    end_time = segment.substr(12, 8);
                }
                
            }
            //Location
            else if(count == 4){
                location = segment;
            }
            //Tag
            else {
                tag = segment;
            }
            //Add to map and reset variables for next line
            if(count == 5){
                id = date + '0';
                int id_count = 0;
                while(event_map.find(id) != event_map.end()){
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
    //Add event_info objects to priotity queue
    for(const auto& pair : event_map){
        pq.push(pair.second);
    }
    //Prints all calender objects
    while(!pq.empty()){
        //cout << pq.top()->get_date() << " " << pq.top()->get_start() << " " << pq.top()->get_title() << endl;
        pq.top()->print();
        pq.pop();
    }

};
