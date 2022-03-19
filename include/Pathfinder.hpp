#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <deque>

namespace py = pybind11;

const int ZERO = 0;
const int ONE = 1;

class Node
{
public:
    Node() : parent(nullptr){};
    Node(const int &I, const int &J,
         const float &G, const float &H, const float &F,
         Node *Parent) : i(I), j(J), g(G), h(H), f(F), parent(Parent){};
    int i;
    int j;
    float g;
    float h;
    float f;
    Node *parent;
};

struct MoreThanByF
{
    bool operator()(const Node *lhs, const Node *rhs) const
    {
        return lhs->f > rhs->f;
    }
};

class Pathfinder
{
public:
    Pathfinder(int cols, int rows);
    ~Pathfinder() {};

    void clearGrid();

    bool setGrid(const py::list &grid);

    bool searchAStar(const int &start_i, const int &start_j,
                     const int &end_i, const int &end_j,
                     const bool &diagonal);

    const std::vector<std::pair<int, int>> &getPath() const;
    const int& getRuns() const {return runs_; }

    int index(const int &i, const int &j) const { return i + g_cols_ * j; }
    virtual bool validIndex(const int &i, const int &j) const;

    virtual bool validCell(const int &i, const int &j) const;
    virtual const int &cellValue(const int &i, const int &j) const;
private:
    std::vector<int> grid_;

    int g_cols_;
    int g_rows_;

    int runs_;
    int nodesUsed_;
    int reusedNodes_;

    std::vector<std::pair<int, int>> resultPathCells_;

    std::vector<Node *> nodeList_;

    std::unordered_map<int, Node *> openList_;

    std::unordered_map<int, bool> closedList_;

    std::priority_queue<Node *, std::vector<Node *>, MoreThanByF> openQueue_;

    Node *newNode(const int &I, const int &J,
                  const float &G, const float &H, const float &F,
                  Node *Parent);
    void cleanUp();

    bool validAdjacent_(const int &i, const int &j, const int &center_i, const int &center_j, const bool &diagonal) const;
};

#endif // PATHFINDER_HPP
