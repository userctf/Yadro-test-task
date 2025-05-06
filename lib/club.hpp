#pragma once

#include <cstddef>
#include <deque>
#include <map>
#include <string>
#include <vector>

namespace club {

class Club {
private:
    struct Client {
        size_t in_time { 0 };
        // if table_no == _tables_count + 1 then it means that client didn't sit down
        size_t table_no { 0 };
    };

    size_t _tables_count;
    size_t _start_time;
    size_t _end_time;
    size_t _price;

    class Table {
    public:
        size_t full_hours { 0 };
        size_t total_time { 0 };
        bool is_free { true };

        void close();
        void open(size_t time);
    };

    std::map<std::string, Client> _clients;
    std::vector<Table> _tables;

    std::deque<std::string> _clients_queue;
    size_t _used_tables = 0;

    void write_event(size_t time, int type, const std::string& data);

    // Generated events
    void apply_type_12(size_t time, int type, const std::string& data, size_t table_no);

    // Input events
    void apply_type_1(size_t time, int type, const std::string& data, size_t table_no);

    void apply_type_2(size_t time, int type, const std::string& data, size_t table_no);

    void apply_type_3(size_t time, int type, const std::string& data, size_t table_no);

    void apply_type_4(size_t time, int type, const std::string data, size_t table_no);

    void kick_all(size_t time);

public:
    Club(size_t tables_count, size_t start_time, size_t end_time, size_t price);

    void apply_event(size_t time, int type, const std::string& data, size_t table_no);

    ~Club();
};

} // namespace club
