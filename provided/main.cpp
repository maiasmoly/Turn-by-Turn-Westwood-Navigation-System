//#include <cstdio>
//#include <iomanip>
//#include <iostream>
//#include <map>
//#include <vector>
//
//#include "geodb.h"
//#include "router.h"
//#include "stops.h"
//#include "tourcmd.h"
//#include "tour_generator.h"

//
//using namespace std;
//
//void print_tour(vector<TourCommand>& tcs)
//{
//    double total_dist = 0;
//    std::string direction;
//    double street_distance = 0;
//
//    cout << "Starting tour...\n";
//
//    for (size_t i = 0; i < tcs.size(); ++i)
//    {
//        if (tcs[i].get_command_type() == TourCommand::commentary)
//        {
//            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
//            cout << tcs[i].get_commentary() << "\n";
//        }
//        else if (tcs[i].get_command_type() == TourCommand::turn)
//        {
//            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
//        }
//        else if (tcs[i].get_command_type() == TourCommand::proceed)
//        {
//            total_dist += tcs[i].get_distance();
//            if (direction.empty())
//                direction = tcs[i].get_direction();
//            street_distance += tcs[i].get_distance();
//            if (i+1 < tcs.size() && tcs[i+1].get_command_type() == TourCommand::proceed
//                && tcs[i+1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
//            {
//                continue;
//            }
//
//            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
//            street_distance = 0;
//            direction.clear();
//        }
//    }
//
//    cout << "Your tour has finished!\n";
//    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
//}
//
//int main(int argc, char *argv[])
//{
//    if (argc != 3)
//    {
//        cout << "usage: BruinTour mapdata.txt stops.txt\n";
//        return 1;
//    }
//
//    GeoDatabase geodb;
//    if (!geodb.load(argv[1]))
//    {
//        cout << "Unable to load map data: " << argv[1] << endl;
//        return 1;
//    }
//
//    Router router(geodb);
//    TourGenerator tg(geodb, router);
//
//    Stops stops;
//    if (!stops.load(argv[2]))
//    {
//        cout << "Unable to load tour data: " << argv[2] << endl;
//        return 1;
//    }
//
//    std::cout << "Routing...\n\n";
//
//    vector<TourCommand> tcs = tg.generate_tour(stops);
//    if (tcs.empty())
//        cout << "Unable to generate tour!\n";
//    else
//        print_tour(tcs);
//}

#include <string>
#include <iostream>

#include "HashMap.h"
#include "GeoDatabase.h"

//int main() {
//    HashMap<std::string> h;
//    h.insert("Maia", "Smolyanov");
//    h.insert("Alex", "Smolyanov");
//    h.insert("Elena", "Smolyanov");
//    h.insert("Alex", ":)");
//    h.insert("hi", ":)");
//    h.insert("hey", ":)");
//    h.insert("yay", ":)");
//    h.insert("lol", ":)");
//    h.insert("banana", ":)");
//    h.insert("a", ":)");
//    h.insert("b", ":)");
//    h.insert("c", ":)");
//    h.insert("d", ":)");
//    assert(*h.find("Maia") == "Smolyanov");
//    assert(h.find("Foo") == nullptr);
//    assert(*h.find("Alex") == ":)");
//    assert(h["Maia"] == "Smolyanov");
//    assert(h["x"] == "");
//    assert(*h.find("x") == "");
//    h["x"] = "hello";
//    assert(*h.find("x") == "hello");
//
//    std::cerr << "Tests passed" << std::endl;
//}

int main() {
    GeoDatabase gdb;
    gdb.load("/Users/maiasmolyanov/Documents/CS_32/Project_4/Project_4/mapdata.txt");
    GeoPoint gp;
    if (gdb.get_poi_location("Saint Sebastian School", gp)) {
        std::cerr << "Loc is: " << gp.to_string() << std::endl;
    } else {
        std::cerr << "none found" << std::endl;
    }
    
    std::vector<GeoPoint> points = gdb.get_connected_points(gp); //GeoPoint("34.0555356","-118.4798135"));
    for (GeoPoint& p : points) {
        std::cerr << "  Connected to: "<< p.to_string() << std::endl;
    }
    std::string street = gdb.get_street_name(GeoPoint("34.0469479", "-118.3754206"),
                                             GeoPoint("34.0468506", "-118.3749202")
                                             );
    std::cerr << "  Street is " << street << std::endl;
    
}
