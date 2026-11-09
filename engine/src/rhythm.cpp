#include "amigo/rhythm.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <utility>
namespace amigo {
namespace {
constexpr std::uint64_t perfect_window = 45, good_window = 90, miss_window = 120;
std::string trim(const std::string& s) {
    const auto a=s.find_first_not_of(" \t\r");
    if(a==std::string::npos) return {};
    return s.substr(a,s.find_last_not_of(" \t\r")-a+1);
}
bool parse_u64(const std::string& s,std::uint64_t& out) {
    try { const auto v=trim(s); std::size_t n=0; auto x=std::stoull(v,&n); if(n!=v.size()) return false; out=x; return true; }
    catch(...) { return false; }
}
}
bool Chart::parse(const std::string& text, Chart& output, std::string& error) {
    Chart parsed; bool header=false,bpm_ok=false; std::istringstream input(text); std::string line; std::size_t line_no=0;
    while(std::getline(input,line)) {
        ++line_no; line=trim(line); if(line.empty()||line[0]=='#') continue;
        std::istringstream row(line); std::string kind; std::getline(row,kind,','); kind=trim(kind);
        if(kind=="AMIGOCHART") {
            std::string v; if(!std::getline(row,v,',')||trim(v)!="1"||header) { error="Line "+std::to_string(line_no)+": expected AMIGOCHART,1"; return false; } header=true;
        } else if(kind=="title") {
            std::getline(row,parsed.title); parsed.title=trim(parsed.title);
        } else if(kind=="bpm") {
            std::string v; std::getline(row,v);
            try { const auto s=trim(v); std::size_t n=0; parsed.bpm=std::stod(s,&n); if(n!=s.size()||!std::isfinite(parsed.bpm)||parsed.bpm<=0.0||parsed.bpm>1000.0) throw std::invalid_argument("bpm"); bpm_ok=true; }
            catch(...) { error="Line "+std::to_string(line_no)+": invalid BPM"; return false; }
        } else if(kind=="note") {
            std::string t,side,z,extra;
            if(!std::getline(row,t,',')||!std::getline(row,side,',')||!std::getline(row,z,',')) { error="Line "+std::to_string(line_no)+": note requires time, side, zone"; return false; }
            if(std::getline(row,extra,',')) { error="Line "+std::to_string(line_no)+": too many note fields"; return false; }
            ChartNote note; if(!parse_u64(t,note.time_ms)) { error="Line "+std::to_string(line_no)+": invalid note time"; return false; }
            side=trim(side); if(side=="left") note.side=MaracaSide::left; else if(side=="right") note.side=MaracaSide::right; else { error="Line "+std::to_string(line_no)+": side must be left or right"; return false; }
            std::uint64_t zone=0; if(!parse_u64(z,zone)||zone>2) { error="Line "+std::to_string(line_no)+": zone must be 0, 1, or 2"; return false; }
            note.zone=static_cast<int>(zone); parsed.notes.push_back(note);
        } else { error="Line "+std::to_string(line_no)+": unknown record"; return false; }
    }
    if(!header) { error="Missing AMIGOCHART,1 header"; return false; }
    if(!bpm_ok) { error="Missing or invalid BPM"; return false; }
    if(parsed.notes.empty()) { error="Chart contains no notes"; return false; }
    for(std::size_t i=1;i<parsed.notes.size();++i) if(parsed.notes[i].time_ms<parsed.notes[i-1].time_ms) { error="Notes must be ordered by time"; return false; }
    output=std::move(parsed); error.clear(); return true;
}
RhythmSession::RhythmSession(Chart chart):chart_(std::move(chart)),states_(chart_.notes.size(),NoteState::pending){}
std::size_t RhythmSession::advance(std::uint64_t now) {
    std::size_t count=0;
    for(std::size_t i=0;i<chart_.notes.size();++i) if(states_[i]==NoteState::pending && now>chart_.notes[i].time_ms+miss_window) {
        states_[i]=NoteState::missed; ++stats_.misses; stats_.combo=0; ++count;
    }
    return count;
}
HitResult RhythmSession::hit(MaracaSide side,int zone,std::uint64_t now) {
    if(zone<0||zone>2) return {};
    std::size_t best=static_cast<std::size_t>(-1); std::uint64_t best_error=miss_window+1;
    for(std::size_t i=0;i<chart_.notes.size();++i) {
        if(states_[i]!=NoteState::pending) continue;
        const auto& n=chart_.notes[i]; if(n.side!=side||n.zone!=zone) continue;
        const auto delta=now>n.time_ms?now-n.time_ms:n.time_ms-now;
        if(delta<=miss_window&&delta<best_error) { best=i; best_error=delta; }
    }
    if(best==static_cast<std::size_t>(-1)) return {};
    states_[best]=NoteState::hit; HitGrade grade;
    if(best_error<=perfect_window) { grade=HitGrade::perfect; ++stats_.perfect; stats_.score+=1000; }
    else if(best_error<=good_window) { grade=HitGrade::good; ++stats_.good; stats_.score+=500; }
    else { grade=HitGrade::miss; ++stats_.misses; stats_.combo=0; }
    if(grade==HitGrade::perfect||grade==HitGrade::good) { ++stats_.combo; stats_.max_combo=std::max(stats_.max_combo,stats_.combo); }
    return {grade,best,best_error};
}
std::size_t RhythmSession::remaining_notes() const noexcept {
    return static_cast<std::size_t>(std::count(states_.begin(),states_.end(),NoteState::pending));
}
} // namespace amigo
