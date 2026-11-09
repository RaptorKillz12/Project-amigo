#include "amigo/rhythm.hpp"
#include <cassert>
#include <string>
int main() {
    const std::string source =
        "AMIGOCHART,1\n"
        "title,Unit Test\n"
        "bpm,120\n"
        "note,1000,left,0\n"
        "note,1500,right,2\n";
    amigo::Chart chart;
    std::string error;
    assert(amigo::Chart::parse(source, chart, error));
    assert(chart.title == "Unit Test");
    assert(chart.notes.size() == 2);
    amigo::RhythmSession session(chart);
    auto first = session.hit(amigo::MaracaSide::left, 0, 1020);
    assert(first.grade == amigo::HitGrade::perfect);
    assert(session.stats().score == 1000);
    assert(session.stats().combo == 1);
    auto second = session.hit(amigo::MaracaSide::right, 2, 1580);
    assert(second.grade == amigo::HitGrade::good);
    assert(session.stats().good == 1);
    assert(session.stats().max_combo == 2);
    assert(session.remaining_notes() == 0);
    amigo::Chart invalid;
    assert(!amigo::Chart::parse("AMIGOCHART,1\nbpm,0\nnote,1,left,0\n", invalid, error));
    return 0;
}
