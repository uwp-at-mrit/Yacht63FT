#include "event.hpp"

#include "dbsystem.hpp"
#include "dbtypes.hpp"

#include "dbmisc.hpp"

using namespace WarGrey::SCADA;

static const char* event_rowids[] = { "uuid" };

static TableColumnInfo event_columns[] = {
    { "uuid", SDT::Integer, nullptr, DB_PRIMARY_KEY | 0 | 0 },
    { "name", SDT::Integer, nullptr, 0 | DB_NOT_NULL | 0 },
    { "timestamp", SDT::Integer, nullptr, 0 | DB_NOT_NULL | 0 },
    { "status_icon", SDT::Integer, nullptr, 0 | DB_NOT_NULL | 0 },
    { "code", SDT::Integer, nullptr, 0 | 0 | 0 },
    { "note", SDT::Text, nullptr, 0 | 0 | 0 },
};

/**************************************************************************************************/
AlarmEvent_pk WarGrey::SCADA::event_identity(AlarmEvent& self) {
    return self.uuid;
}

AlarmEvent WarGrey::SCADA::make_event(std::optional<Integer> name, std::optional<Integer> status, std::optional<Integer> code, std::optional<Text> note) {
    AlarmEvent self;

    default_event(self, name, status, code, note);

    return self;
}

void WarGrey::SCADA::default_event(AlarmEvent& self, std::optional<Integer> name, std::optional<Integer> status, std::optional<Integer> code, std::optional<Text> note) {
    self.uuid = pk64_timestamp();
    if (name.has_value()) { self.name = name.value(); }
    self.timestamp = current_milliseconds();
    if (status.has_value()) { self.status = status.value(); }
    if (code.has_value()) { self.code = code.value(); }
    if (note.has_value()) { self.note = note.value(); }
}

void WarGrey::SCADA::refresh_event(AlarmEvent& self) {
}

void WarGrey::SCADA::store_event(AlarmEvent& self, IPreparedStatement* stmt) {
    stmt->bind_parameter(0U, self.uuid);
    stmt->bind_parameter(1U, self.name);
    stmt->bind_parameter(2U, self.timestamp);
    stmt->bind_parameter(3U, self.status);
    stmt->bind_parameter(4U, self.code);
    stmt->bind_parameter(5U, self.note);
}

void WarGrey::SCADA::restore_event(AlarmEvent& self, IPreparedStatement* stmt) {
    self.uuid = stmt->column_int64(0U);
    self.name = stmt->column_int64(1U);
    self.timestamp = stmt->column_int64(2U);
    self.status = stmt->column_int64(3U);
    self.code = stmt->column_maybe_int64(4U);
    self.note = stmt->column_maybe_text(5U);
}

/**************************************************************************************************/
void WarGrey::SCADA::create_event(IDBSystem* dbc, bool if_not_exists) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    std::string sql = vsql->create_table("event", event_rowids, sizeof(event_rowids)/sizeof(char*), if_not_exists);

    dbc->exec(sql);
}

void WarGrey::SCADA::insert_event(IDBSystem* dbc, AlarmEvent& self, bool replace) {
    insert_event(dbc, &self, 1, replace);
}

void WarGrey::SCADA::insert_event(IDBSystem* dbc, AlarmEvent* selves, size_t count, bool replace) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    std::string sql = vsql->insert_into("event", replace);
    IPreparedStatement* stmt = dbc->prepare(sql);

    if (stmt != nullptr) {
        for (size_t i = 0; i < count; i ++) {
            store_event(selves[i], stmt);

            dbc->exec(stmt);
            stmt->reset(true);
        }

        delete stmt;
    }
}

std::list<AlarmEvent_pk> WarGrey::SCADA::list_event(IDBSystem* dbc, uint64 limit, uint64 offset, event order_by, bool asc) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    const char* colname = ((order_by == event::_) ? nullptr : event_columns[static_cast<unsigned int>(order_by)].name);
    std::string sql = vsql->select_from("event", colname, asc, event_rowids, sizeof(event_rowids)/sizeof(char*), limit, offset);
    IPreparedStatement* stmt = dbc->prepare(sql);
    std::list<AlarmEvent_pk> queries;

    if (stmt != nullptr) {
        while(stmt->step()) {
            queries.push_back(stmt->column_int64(0U));
        }

        delete stmt;
    }

    return queries;
}

std::list<AlarmEvent> WarGrey::SCADA::select_event(IDBSystem* dbc, uint64 limit, uint64 offset, event order_by, bool asc) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    const char* colname = ((order_by == event::_) ? nullptr : event_columns[static_cast<unsigned int>(order_by)].name);
    std::string sql = vsql->select_from("event", colname, asc, limit, offset);
    IPreparedStatement* stmt = dbc->prepare(sql);
    std::list<AlarmEvent> queries;

    if (stmt != nullptr) {
        AlarmEvent self;

        while(stmt->step()) {
            restore_event(self, stmt);
            queries.push_back(self);
        }

        delete stmt;
    }

    return queries;
}

std::optional<AlarmEvent> WarGrey::SCADA::seek_event(IDBSystem* dbc, AlarmEvent_pk where) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    std::string sql = vsql->seek_from("event", event_rowids, sizeof(event_rowids)/sizeof(char*));
    IPreparedStatement* stmt = dbc->prepare(sql);
    std::optional<AlarmEvent> query;

    if (stmt != nullptr) {
        AlarmEvent self;

        stmt->bind_parameter(0U, where);

        if (stmt->step()) {
            restore_event(self, stmt);
            query = self;
        }

        delete stmt;
    }

    return query;
}

void WarGrey::SCADA::update_event(IDBSystem* dbc, AlarmEvent& self, bool refresh) {
    update_event(dbc, &self, 1);
}

void WarGrey::SCADA::update_event(IDBSystem* dbc, AlarmEvent* selves, size_t count, bool refresh) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    std::string sql = vsql->update_set("event", event_rowids, sizeof(event_rowids)/sizeof(char*));
    IPreparedStatement* stmt = dbc->prepare(sql);

    if (stmt != nullptr) {
        for (size_t i = 0; i < count; i ++) {
            if (refresh) {
                refresh_event(selves[i]);
            }

            stmt->bind_parameter(5U, selves[i].uuid);

            stmt->bind_parameter(0U, selves[i].name);
            stmt->bind_parameter(1U, selves[i].timestamp);
            stmt->bind_parameter(2U, selves[i].status);
            stmt->bind_parameter(3U, selves[i].code);
            stmt->bind_parameter(4U, selves[i].note);

            dbc->exec(stmt);
            stmt->reset(true);
        }

        delete stmt;
    }
}

void WarGrey::SCADA::delete_event(IDBSystem* dbc, AlarmEvent_pk& where) {
    delete_event(dbc, &where, 1);
}

void WarGrey::SCADA::delete_event(IDBSystem* dbc, AlarmEvent_pk* wheres, size_t count) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    std::string sql = vsql->delete_from("event", event_rowids, sizeof(event_rowids)/sizeof(char*));
    IPreparedStatement* stmt = dbc->prepare(sql);

    if (stmt != nullptr) {
        for (size_t i = 0; i < count; i ++) {
            stmt->bind_parameter(0U, wheres[i]);

            dbc->exec(stmt);
            stmt->reset(true);
        }

        delete stmt;
    }
}

void WarGrey::SCADA::drop_event(IDBSystem* dbc) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    std::string sql = vsql->drop_table("event");

    dbc->exec(sql);
}

/**************************************************************************************************/
double WarGrey::SCADA::event_average(WarGrey::SCADA::IDBSystem* dbc, event column, bool distinct) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    const char* colname = ((column == event::_) ? nullptr : event_columns[static_cast<unsigned int>(column)].name);

    return dbc->query_double(vsql->table_average("event", colname, distinct));
}

int64 WarGrey::SCADA::event_count(WarGrey::SCADA::IDBSystem* dbc, event column, bool distinct) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    const char* colname = ((column == event::_) ? nullptr : event_columns[static_cast<unsigned int>(column)].name);

    return dbc->query_int64(vsql->table_count("event", colname, distinct));
}

std::optional<double> WarGrey::SCADA::event_max(WarGrey::SCADA::IDBSystem* dbc, event column, bool distinct) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    const char* colname = ((column == event::_) ? nullptr : event_columns[static_cast<unsigned int>(column)].name);

    return dbc->query_maybe_double(vsql->table_max("event", colname, distinct));
}

std::optional<double> WarGrey::SCADA::event_min(WarGrey::SCADA::IDBSystem* dbc, event column, bool distinct) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    const char* colname = ((column == event::_) ? nullptr : event_columns[static_cast<unsigned int>(column)].name);

    return dbc->query_maybe_double(vsql->table_min("event", colname, distinct));
}

std::optional<double> WarGrey::SCADA::event_sum(WarGrey::SCADA::IDBSystem* dbc, event column, bool distinct) {
    IVirtualSQL* vsql = dbc->make_sql_factory(event_columns);
    const char* colname = ((column == event::_) ? nullptr : event_columns[static_cast<unsigned int>(column)].name);

    return dbc->query_maybe_double(vsql->table_sum("event", colname, distinct));
}

