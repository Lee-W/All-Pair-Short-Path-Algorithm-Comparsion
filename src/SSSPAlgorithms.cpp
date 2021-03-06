#include "SSSPAlgorithms.h"

bool SSSPAlgorithms::arcCmp(Arc a1, Arc a2)
{
    if (a1.from == a2.from) {
        if (a1.to < a2.to)
            return a1.arcLength < a2.arcLength;
        return a1.to < a2.to;
    }
    return a1.from < a2.from;
}

void SSSPAlgorithms::initial(int n, vector<Arc> arcs)
{
    nodeNum = n;
    reset();

    network.resize(nodeNum + 1);

    sort(arcs.begin(), arcs.end(), arcCmp);
    int preFrom = -1;
    int preTo = -1;
    for (auto arc : arcs) {
        if (arc.from == preFrom && arc.to == preTo)
            continue;
        else {
            network[arc.from].push_back(arc);
            preFrom = arc.from;
            preTo = arc.to;
        }
    }
}

void SSSPAlgorithms::reset()
{
    dis.clear();
    dis.resize(nodeNum + 1, INF);
    pre.clear();
    pre.resize(nodeNum + 1, -1);
}

void SSSPAlgorithms::DijkstraWithBinaryHeap(int source)
{
    reset();

    clock_t startTime = clock();
    relaxNum = 0;

    vector<bool> visited;
    visited.resize(nodeNum+1, false);

    priority_queue<Node, vector<Node>, NodeComparator> pq;
    dis[source] = 0;
    pre[source] = source;
    pq.push(Node(source, dis[source]));

    Node heapTop;
    int tempDis;
    int index;
    while (!pq.empty()) {
        heapTop = pq.top();
        pq.pop();
        if (!visited[heapTop.index]) {
            visited[heapTop.index] = true;

            // Relaxation
            for (auto i : network[heapTop.index]) {
                if (!visited[i.to]) {
                    relaxNum++;
                    tempDis = dis[heapTop.index] + i.arcLength;
                    if (tempDis < dis[i.to]) {
                        dis[i.to] = tempDis;
                        pre[i.to] = heapTop.index;
                    }
                    pq.push(Node(i.to, tempDis));
                }
            }
        }
    }
    processTime = clock() - startTime;
}

void SSSPAlgorithms::Dial(int source)
{
    reset();

    clock_t startTime = clock();
    relaxNum = 0;

    vector<bool> visited;
    visited.resize(nodeNum+1, false);

    vector<list<int> > indexBucket;
    int maxBucketSize = countMaxBucketSize();
    indexBucket.resize(maxBucketSize+1);

    dis[source] = 0;
    pre[source] = source;
    indexBucket[dis[source]].push_back(source);

    int curMin = 0;
    int curNodeIndex;
    int tempDis;
    while (curMin < maxBucketSize+1) {
        if (!indexBucket[curMin].empty()) {
            curNodeIndex = indexBucket[curMin].back();
            indexBucket[curMin].pop_back();

            if (!visited[curNodeIndex]) {
                visited[curNodeIndex] = true;

                for (auto i : network[curNodeIndex]) {
                    relaxNum++;

                    tempDis = dis[curNodeIndex] + i.arcLength;
                    if (tempDis < dis[i.to]) {
                        // In case that bucket is not large enough
                        if (tempDis > indexBucket.size())
                            indexBucket.resize(tempDis + 1);

                        indexBucket[tempDis].push_back(i.to);

                        dis[i.to] = tempDis;
                        pre[i.to] = curNodeIndex;
                    }
                }
            }
        } else {
            curMin++;
        }
    }

    processTime = clock() - startTime;
}

// Count the sum of the first ecountered numbers since bucket size would never
// be large than it
int SSSPAlgorithms::countMaxBucketSize()
{
    int maxBucketSize = 0;

    int counter = 0;
    for (auto i : network) {
        for (auto j : i) {
            maxBucketSize += j.arcLength;
            counter++;
        }
        if (counter > nodeNum)
            break;
    }
    return maxBucketSize;
}

void SSSPAlgorithms::vectorDelete(vector<int>& v, int value)
{
    v.erase(remove(v.begin(), v.end(), value), v.end());
}

// Bellman Ford with Dequeue
void SSSPAlgorithms::PAPE(int source)
{
    reset();
    clock_t startTime = clock();
    relaxNum = 0;

    deque<int> dq;
    vector<bool> inqueue(nodeNum + 1, false);
    vector<bool> isUpdated(nodeNum + 1, false);

    dis[source] = 0;
    inqueue[source] = true;
    isUpdated[source] = true;
    dq.push_front(source);

    while (!dq.empty()) {
        int cur = dq.front();
        inqueue[cur] = false;
        dq.pop_front();

        for (auto arc : network[cur]) {
            int des = arc.to;
            int len = arc.arcLength;

            relaxNum++;
            if (dis[cur] + len < dis[des]) {
                dis[des] = dis[cur] + len;
                pre[des] = cur;

                if (isUpdated[des]) {
                    dq.push_front(des);
                } else {
                    dq.push_back(des);
                    isUpdated[des] = true;
                }
                inqueue[des];
            }
        }
    }

    processTime = clock() - startTime;
}

// Bellman Ford with Queue
void SSSPAlgorithms::SPFA(int source)
{
    reset();

    clock_t startTime = clock();
    relaxNum = 0;

    queue<int> q;
    vector<bool> inqueue(nodeNum + 1, false);

    dis[source] = 0;
    inqueue[source] = true;
    q.push(source);

    while (!q.empty()) {
        int cur = q.front();
        inqueue[cur] = false;
        q.pop();

        for (auto arc : network[cur]) {
            int des = arc.to;
            int len = arc.arcLength;

            relaxNum++;
            if (dis[cur] + len < dis[des]) {
                dis[des] = dis[cur] + len;
                pre[des] = cur;
                q.push(des);
                inqueue[des] = true;
            }
        }
    }

    processTime = clock() - startTime;
}

vector<int> SSSPAlgorithms::getDis()
{
    return dis;
}

vector<int> SSSPAlgorithms::getPre()
{
    return pre;
}

clock_t SSSPAlgorithms::getProcessTime()
{
    return processTime;
}

int SSSPAlgorithms::getRelaxNum()
{
    return relaxNum;
}
