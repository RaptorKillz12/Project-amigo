#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace amigo {
enum class MaracaSide { left, right };
enum class HitGrade { perfect, good, miss, no_match };
struct ChartNote { std::uint64_t time_ms{0}; MaracaSide side{MaracaSide::left}; int zone{0}; };
struct Chart {
    std::string title;
    double bpm{0.0};
    std::vector<ChartNote> notes;
    // Format: AMIGOCHART,1; title,<text>; bpm,<number>; note,<time_ms>,<left|right>,<0|1|2>.
    static bool parse(const std::string& text, Chart& output, std::string& error);
};
struct HitResult { HitGrade grade{HitGrade::no_match}; std::size_t note_index{static_cast<std::size_t>(-1)}; std::uint64_t timing_error_ms{0}; };
struct RhythmStats { std::uint64_t score{0}; std::size_t perfect{0}, good{0}, misses{0}, combo{0}, max_combo{0}; };
class RhythmSession {
public:
    explicit RhythmSession(Chart chart);
    std::size_t advance(std::uint64_t now_ms);
    HitResult hit(MaracaSide side, int zone, std::uint64_t now_ms);
    const RhythmStats& stats() const noexcept { return stats_; }
    std::size_t remaining_notes() const noexcept;
    const Chart& chart() const noexcept { return chart_; }
private:
    enum class NoteState { pending, hit, missed };
    Chart chart_;
    std::vector<NoteState> states_;
    RhythmStats stats_;
};
} // namespace amigo
