#include "core/Graph.h"
#include "core/Util.h"
#include <unordered_set>

void Graph::build(const std::vector<Flight> &flights, int dow)
{
    adj_.clear();
    std::unordered_set<std::string> seenEdge; 
    for (const auto &f : flights)
    {
        if (!operatesOn(f.days, dow))
            continue;
        std::string key = f.from + "->" + f.to;
        if (seenEdge.insert(key).second)
        {
            adj_[f.from].push_back(Edge{f.to, &f, 1});
        }
        if (!adj_.count(f.to))
            adj_[f.to] = {};
        if (!adj_.count(f.from))
            adj_[f.from] = adj_[f.from];
    }
}

const std::vector<Edge> &Graph::neighbors(const std::string &code) const
{
    static const std::vector<Edge> empty;
    auto it = adj_.find(code);
    if (it == adj_.end())
        return empty;
    return it->second;
}