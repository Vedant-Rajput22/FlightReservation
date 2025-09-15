#include "core/DijkstraRouter.h"
#include <queue>
#include <unordered_map>

struct NodeState
{
    std::string code;
    int dist;
    std::vector<std::string> path;
    bool operator<(const NodeState &other) const { return dist > other.dist; }
};

std::vector<std::vector<std::string>> DijkstraRouter::fewestStops(
    const Graph &g,
    const std::string &src,
    const std::string &dst,
    int topK) const
{
    std::priority_queue<NodeState> pq;
    std::unordered_map<std::string, int> best;

    pq.push(NodeState{src, 0, {src}});
    std::vector<std::vector<std::string>> results;

    while (!pq.empty() && (int)results.size() < topK)
    {
        auto cur = pq.top();
        pq.pop();
        if (best.count(cur.code) && best[cur.code] < cur.dist)
            continue;
        if (cur.code == dst)
        {
            results.push_back(cur.path);
            continue;
        }
        for (const auto &e : g.neighbors(cur.code))
        {
            int nd = cur.dist + e.weight;
            if (!best.count(e.to) || nd <= best[e.to])
            {
                best[e.to] = nd;
                auto np = cur.path;
                np.push_back(e.to);
                pq.push(NodeState{e.to, nd, std::move(np)});
            }
        }
    }
    return results;
}