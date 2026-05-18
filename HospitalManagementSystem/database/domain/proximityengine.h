#pragma once
#include <map>
#include <string>
#include <vector>

// ProximityEngine calculates a match score between a hospital and a patient's
// location sector using a BFS over the Islamabad sector adjacency graph.
// Demonstrates: Encapsulation, graph algorithms
class ProximityEngine {
public:
    ProximityEngine();

    // Returns a match score (0–100) for the given hospital and patient sector.
    // 95 = direct match, 80 = 1 hop, 65 = 2 hops, 52 = 3 hops, 40 = farther/unreachable
    int score(const std::string& hospitalName,
              const std::string& patientSector) const;

private:
    // Builds the Islamabad sector adjacency graph
    void buildGraph();

    // BFS returning hop distance from 'from' to 'to'; -1 if unreachable
    int bfs(const std::string& from, const std::string& to) const;

    // Adjacency graph: sector → list of adjacent sectors
    std::map<std::string, std::vector<std::string>> graph;

    // Hospital name → list of associated sectors
    std::map<std::string, std::vector<std::string>> locMap;
};
