#include "club.hpp"

#include <iostream>

#define EVENT_ERR_CODE 13

namespace {
void write_time(size_t time)
{
    printf("%02d:%02d", time / 60, time % 60);
}
}

namespace club {
void Club::write_event(size_t time, int type, const std::string& data)
{
    write_time(time);
    std::cout << " " << type << " " << data << '\n';
}

void Club::apply_type_12(size_t time, int type, const std::string& data, size_t table_no)

{
    _clients[data] = { time, table_no };

    _used_tables++;
    _tables[table_no].close();

    write_event(time, 12, data + " " + std::to_string(table_no + 1));
}

void Club::apply_type_1(size_t time, int type, const std::string& data, size_t table_no)
{
    write_event(time, type, data);
    if (time >= _start_time && time <= _end_time) {
        if (_clients.find(data) == _clients.end()) {
            _clients[data] = { time, _tables_count + 1 };
        } else {
            write_event(time, EVENT_ERR_CODE, "YouShallNotPass");
        }
    } else {
        write_event(time, EVENT_ERR_CODE, "NotOpenYet");
    }
}

void Club::apply_type_2(size_t time, int type, const std::string& data, size_t table_no)
{
    write_event(time, type, data + " " + std::to_string(table_no + 1));
    auto client = _clients.find(data);
    if (client != _clients.end()) {
        size_t curr_table_no = client->second.table_no;
        if (_tables[table_no].is_free) {

            client->second.table_no = table_no;
            _tables[table_no].close();
            if (curr_table_no == _tables_count + 1) {
                _used_tables++;
            } else { // change seat
                _tables[curr_table_no].open(time - client->second.in_time);
            }
            client->second.in_time = time;
        } else {
            write_event(time, EVENT_ERR_CODE, "PlaceIsBusy");
        }
    } else {
        write_event(time, EVENT_ERR_CODE, "ClientUnknown");
    }
}

void Club::apply_type_3(size_t time, int type, const std::string& data, size_t table_no)
{
    write_event(time, type, data);

    if (_used_tables != _tables.size()) {
        write_event(time, EVENT_ERR_CODE, "ICanWaitNoLonger!");
    } else if (_clients_queue.size() > _tables_count) {
        // client go out
        write_event(time, 11, data);
    } else {
        _clients_queue.push_back(data);
    }
}

void Club::apply_type_4(size_t time, int type, const std::string data, size_t table_no)

{
    write_event(time, type, data);

    auto client = _clients.find(data);
    if (client != _clients.end()) {
        size_t curr_table_no = client->second.table_no;
        _used_tables--;
        _tables[curr_table_no].open(time - client->second.in_time);
        _clients.erase(client);
        if (!_clients_queue.empty()) {
            apply_type_12(time, 4, _clients_queue.front(), curr_table_no);
            _clients_queue.pop_front();
        }
    } else {
        write_event(time, EVENT_ERR_CODE, "ClientUnknown");
    }
}

void Club::kick_all(size_t time)
{

    for (const auto& client : _clients) {
        write_event(time, 11, client.first);
        if (client.second.table_no != _tables_count + 1) {
            _tables[client.second.table_no].open(time - client.second.in_time);
        }
    }

    write_time(_end_time);
    std::cout << '\n';

    for (size_t i { 0 }; i < _tables.size(); ++i) {
        std::cout << (i + 1) << " " << _tables[i].full_hours * _price << " ";
        write_time(_tables[i].total_time);
        std::cout << '\n';
    }
}

Club::Club(size_t tables_count, size_t start_time, size_t end_time, size_t price)
{
    _tables_count = tables_count;
    _start_time = start_time;
    _end_time = end_time;
    _price = price;

    _tables = std::vector<Table>(tables_count);
    write_time(_start_time);
    std::cout << '\n';
}

void Club::apply_event(size_t time, int type, const std::string& data, size_t table_no)
{
    switch (type) {
    case 1:
        apply_type_1(time, type, data, table_no);
        break;
    case 2:
        apply_type_2(time, type, data, table_no);
        break;
    case 3:
        apply_type_3(time, type, data, table_no);
        break;
    case 4:
        apply_type_4(time, type, data, table_no);
        break;
    default:
        break;
    }
}

Club::~Club()
{

    kick_all(_end_time);
}

void Club::Table::close()
{
    is_free = false;
}

void Club::Table::open(size_t time)
{
    // round to top
    full_hours += (time + 60 - 1) / 60;
    total_time += time;
    is_free = true;
}

} // namespace club
