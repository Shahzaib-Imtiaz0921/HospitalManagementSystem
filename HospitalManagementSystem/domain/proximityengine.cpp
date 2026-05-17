#include "proximityengine.h"
#include <climits>
#include <vector>

// ── Constructor ───────────────────────────────────────────────────────────────

ProximityEngine::ProximityEngine() {
    buildGraph();

    // Populate locMap for all 7 hospitals (ported from hospital_management.cpp)
    locMap["PIMS"]                         = { "G-8", "G-7", "G-6" };
    locMap["Polyclinic Hospital"]          = { "G-6", "G-7", "Blue Area" };
    locMap["Shifa International Hospital"] = { "H-8", "H-9", "G-10" };
    locMap["Maroof Hospital"]              = { "F-10", "F-11", "F-8" };
    locMap["Ali Medical Centre"]           = { "F-8", "F-7", "G-8" };
    locMap["Kulsum International Hospital"]= { "F-8", "F-7", "Blue Area" };
    locMap["NESCOM Hospital"]              = { "H-11", "G-13", "H-10" };
}

// ── buildGraph ────────────────────────────────────────────────────────────────
// Full Islamabad sector adjacency graph, ported exactly from hospital_management.cpp

void ProximityEngine::buildGraph() {
    graph["G-6"]       = { "G-7", "F-6", "F-7", "Blue Area" };
    graph["G-7"]       = { "G-6", "G-8", "F-7", "F-8" };
    graph["G-8"]       = { "G-7", "G-9", "F-8", "H-8" };
    graph["G-9"]       = { "G-8", "G-10", "F-9", "H-9" };
    graph["G-10"]      = { "G-9", "G-11", "F-10", "H-10" };
    graph["G-11"]      = { "G-10", "G-12", "F-11", "H-11" };
    graph["G-12"]      = { "G-11", "G-13", "H-12" };
    graph["G-13"]      = { "G-12", "H-13", "H-11" };
    graph["F-6"]       = { "F-7", "G-6", "Blue Area" };
    graph["F-7"]       = { "F-6", "F-8", "G-7", "Blue Area" };
    graph["F-8"]       = { "F-7", "F-9", "G-8" };
    graph["F-9"]       = { "F-8", "F-10", "G-9" };
    graph["F-10"]      = { "F-9", "F-11", "G-10" };
    graph["F-11"]      = { "F-10", "F-12", "G-11" };
    graph["F-12"]      = { "F-11", "G-12" };
    graph["H-8"]       = { "H-9", "G-8" };
    graph["H-9"]       = { "H-8", "H-10", "G-9" };
    graph["H-10"]      = { "H-9", "H-11", "G-10" };
    graph["H-11"]      = { "H-10", "H-12", "G-11", "G-13" };
    graph["H-12"]      = { "H-11", "G-12" };
    graph["H-13"]      = { "G-13" };
    graph["Blue Area"] = { "F-6", "F-7", "G-6", "G-7" };
    graph["Saddar"]    = { "G-6", "G-7", "Blue Area" };
    graph["I-8"]       = { "H-8" };
}

// ── bfs ───────────────────────────────────────────────────────────────────────
// Returns the BFS hop distance from 'from' to 'to', or -1 if unreachable.

int ProximityEngine::bfs(const std::string& from, const std::string& to) const {
    if (from == to) return 0;

    std::map<std::string, int> visited;
    std::vector<std::string>   queue;

    queue.push_back(from);
    visited[from] = 0;

    for (std::size_t head = 0; head < queue.size(); ++head) {
        const std::string& cur = queue[head];
        int dist = visited[cur];

        auto it = graph.find(cur);
        if (it == graph.end()) continue;

        for (const auto& nb : it->second) {
            if (visited.find(nb) == visited.end()) {
                visited[nb] = dist + 1;
                if (nb == to) return dist + 1;
                queue.push_back(nb);
            }
        }
    }
    return -1;  // unreachable
}

// ── score ─────────────────────────────────────────────────────────────────────
// Returns a match score (0–100) for the given hospital and patient sector.

int ProximityEngine::score(const std::string& hospitalName,
                           const std::string& patientSector) const {
    auto it = locMap.find(hospitalName);
    if (it == locMap.end()) return 40;

    // Direct match in locMap → 95
    for (const auto& s : it->second) {
        if (s == patientSector) return 95;
    }

    // BFS: find minimum distance from patientSector to any of the hospital's sectors
    int minDist = INT_MAX;
    for (const auto& s : it->second) {
        int d = bfs(patientSector, s);
        if (d >= 0 && d < minDist) {
            minDist = d;
        }
    }

    if (minDist == INT_MAX) return 40;
    if (minDist == 1)       return 80;
    if (minDist == 2)       return 65;
    if (minDist == 3)       return 52;
    return 40;
}
