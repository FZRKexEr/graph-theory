#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
typedef pair<ll, int> pli;
const ll INF = 1e18;
const ll LARGE = 1e12;
int n, m;
vector<bool> vis;
vector<int> head, nxt, from, to, pre;
vector<ll> raw_cap, cap, cost, p, dis;
priority_queue<pli, vector<pli>, greater<pli> > q;
void add(int u, int v, ll f, ll w)
{
    nxt.push_back(head[u]);
    head[u] = to.size();
    from.push_back(u);
    to.push_back(v);
    raw_cap.push_back(f);
    cap.push_back(0);
    cost.push_back(w);
}
void add_edge(int u, int v, ll f, ll w)
{
    add(u, v, f, w);
    add(v, u, 0, -w);
}
ll c(int id)
{
    return p[from[id]] + cost[id] - p[to[id]];
}
void dijkstra(int s)
{
    vis.assign(n + 2, false);
    dis.assign(n + 2, INF);
    pre.assign(n + 2, -1);
    dis[s] = 0;
    q.push(pli(0, s));
    while (!q.empty())
    {
        int u = q.top().second;
        ll w = q.top().first;
        q.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (int i = head[u]; ~i; i = nxt[i])
        {
            int v = to[i];
            if (cap[i] && dis[v] > w + c(i))
            {
                dis[v] = w + c(i);
                pre[v] = i;
                q.push(pli(dis[v], v));
            }
        }
    }
}
void add_one_cap(int id)
{
    int u = from[id];
    int v = to[id];
    if (cap[id])
    {
        ++cap[id];
        return;
    }
    dijkstra(v);
    if (dis[u] < INF && dis[u] + c(id) < 0)
    {
        ++cap[id ^ 1];
        while (u != v)
        {
            int x = pre[u];
            --cap[x];
            ++cap[x ^ 1];
            u = from[x];
        }
    }
    else ++cap[id];
    ll max_dis = 0;
    ll cur_len = c(id);
    for (int i = 1; i <= n; ++i) if (dis[i] < INF) max_dis = max(max_dis, dis[i]);
    for (int i = 1; i <= n; ++i) p[i] += dis[i] < INF ? dis[i] : max_dis + max(0ll, -cur_len);
    dijkstra(n + 1);
    for (int i = 1; i <= n; ++i) p[i] += dis[i];
}
int main()
{
    int s, t;
    cin >> n >> m >> s >> t;
    head.resize(n + 2, -1);
    p.resize(n + 2, 0);
    for (int i = 1; i <= m; ++i)
    {
        ll u, v, f, w;
        cin >> u >> v >> f >> w;
        add_edge(u, v, f, w);
    }
    add_edge(t, s, LARGE, -LARGE);
    for (int i = 1; i <= n; ++i)
    {
        add_edge(n + 1, i, 0, 0);
        cap[to.size() - 2] = 1;
    }
    for (int i = 40; i >= 0; --i)
    {
        for (int j = 0; j <= m * 2 + 1; ++j) cap[j] <<= 1;
        for (int j = 0; j <= m * 2; j += 2)
        {
            if ((raw_cap[j] >> i) & 1)
            {
                add_one_cap(j);
            }
        }
    }
    ll min_cost = 0;
    for (int i = 0; i < m; ++i) min_cost += cap[i << 1 | 1] * cost[i << 1];
    cout << cap[m << 1 | 1] << ' ' << min_cost;
    return 0;
}
