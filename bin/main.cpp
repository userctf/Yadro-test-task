#include <fstream>
#include <iostream>
#include <lib/club.hpp>

int main(int argc, char const* argv[])
{

    if (argc != 2) {
        std::cerr << "USAGE: task.exe <test_file>" << std::endl;
        return 1;
    }
    std::ifstream input(argv[1], std::ios::in);

    if (!input) {
        std::cerr << "Can't open file: " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    std::getline(input, line);
    int N;
    if (sscanf(line.c_str(), "%d", &N) != 1) {
        std::cerr << line << '\n';
        return 1;
    }

    std::getline(input, line);
    int start_h, start_m, end_h, end_m;
    if (sscanf(line.c_str(), "%d:%d %d:%d", &start_h, &start_m, &end_h, &end_m) != 4) {
        std::cerr << line << '\n';
        return 1;
    }

    std::getline(input, line);
    int price;
    if (sscanf(line.c_str(), "%d", &price) != 1) {
        std::cerr << line << '\n';
        return 1;
    }
    club::Club clb(N, start_h * 60 + start_m, end_h * 60 + end_m, price);

    int event_h, event_m, event_id, event_table_no;
    char event_body[128];
    char event_name[128];

    while (std::getline(input, line)) {
        if (sscanf(line.c_str(), "%d:%d %d %[^\n]", &event_h, &event_m, &event_id, &event_body) != 4) {
            std::cerr << line << '\n';
            return 1;
        }
        if (event_id == 2) {
            if (sscanf(event_body, "%s %d", &event_name, &event_table_no) != 2) {
                std::cerr << line << '\n';
                return 1;
            } else {
                clb.apply_event(event_h * 60 + event_m, event_id, event_name, event_table_no - 1);
            }
        } else {
            clb.apply_event(event_h * 60 + event_m, event_id, event_body, 0);
        }
    }

    input.close();
    return 0;
}
