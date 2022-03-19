#include "Pathfinder.hpp"

Pathfinder::Pathfinder(int cols, int rows) : g_cols_(cols), g_rows_(rows)
{
    grid_.resize(g_cols_ * g_rows_);
}

void Pathfinder::clearGrid()
{
    std::fill(grid_.begin(), grid_.end(), ONE);
}

bool Pathfinder::setGrid(const py::list &grid)
{
    if (grid.size() != grid_.size())
        return false;

    for (int i = 0; i < grid.size(); i++)
    {
        grid_[i] = grid[i].cast<int>();
    }

    return true;
}

bool Pathfinder::searchAStar(const int &start_i, const int &start_j,
                             const int &end_i, const int &end_j,
                             const bool &diagonal)
{
    runs_ = 0;

    if (!(validIndex(start_i, start_j) && validIndex(end_i, end_j)))
        return false;

    if ((start_i == end_i) && (start_j == end_j))
        return false;

    // Clear the data structures
    openQueue_ = std::priority_queue<Node *, std::vector<Node *>, MoreThanByF>();
    openList_.clear();
    closedList_.clear();

    // Add start node to open list
    Node *start = newNode(start_i, start_j, 0, 0, 0, nullptr);
    openList_[index(start_i, start_j)] = start;
    openQueue_.push(start);

    Node *currentNode;
    float child_g, child_h, child_f;
    int currentNodeIndex, child_i, child_j, childIndex;
    int endIndex = index(end_i, end_j);
    reusedNodes_ = 0;
    Node *newChild;
    std::unordered_map<int, bool>::iterator closedSearch;
    std::unordered_map<int, Node *>::iterator foundNode;
    while (!openQueue_.empty())
    {
        runs_ += 1;

        // Pop the openNode with lowest f
        currentNode = openQueue_.top();
        openQueue_.pop();
        currentNodeIndex = index(currentNode->i, currentNode->j);

        // Make sure current node has not been closed yet
        closedSearch = closedList_.find(currentNodeIndex);
        if (closedSearch == closedList_.end())
        {
            closedList_[currentNodeIndex] = true;

            if (currentNodeIndex == endIndex)
            {
                // Reached the end backtrack and build the path from currentNode
                resultPathCells_.clear();
                while (currentNode != nullptr)
                {
                    resultPathCells_.insert(resultPathCells_.begin(), std::pair<int, int>(
                                                                          currentNode->i,
                                                                          currentNode->j));
                    currentNode = currentNode->parent;
                }

                cleanUp();
                return true;
            }

            // Look at all adjacent nodes
            for (int ci = -1; ci < 2; ci++)
            {
                for (int cj = -1; cj < 2; cj++)
                {
                    if (validAdjacent_(ci, cj, currentNode->i, currentNode->j, diagonal))
                    {
                        child_i = currentNode->i + ci;
                        child_j = currentNode->j + cj;
                        childIndex = index(child_i, child_j);
                        // Check to see if child node is closed
                        closedSearch = closedList_.find(childIndex);
                        if (closedSearch == closedList_.end())
                        {
                            child_g = currentNode->g + 1;
                            child_h = (end_i - child_i) * (end_i - child_i) + (end_j - child_j) * (end_j - child_j);
                            child_f = child_g + child_h;
                            foundNode = openList_.find(childIndex);
                            if (foundNode == openList_.end())
                            {
                                // Add child node to open list
                                newChild = newNode(
                                    child_i,
                                    child_j,
                                    child_g,
                                    child_h,
                                    child_f,
                                    currentNode);
                                openList_[index(child_i, child_j)] = newChild;
                                openQueue_.push(newChild);
                            }
                            else
                            {
                                if (foundNode->second->g > child_g)
                                {
                                    // if current child is furthur from origin than the one in
                                    // the open list, switch to current child
                                    foundNode->second->g = child_g;
                                    foundNode->second->h = child_h;
                                    foundNode->second->f = child_f;
                                    foundNode->second->parent = currentNode;
                                    openQueue_.push(foundNode->second);
                                    reusedNodes_ += 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    cleanUp();
    return false;
}

const std::vector<std::pair<int, int>> &Pathfinder::getPath() const
{
    return resultPathCells_;
}

bool Pathfinder::validIndex(const int &i, const int &j) const
{
    if (i < 0)
        return false;
    if (i > g_cols_ - 1)
        return false;
    if (j < 0)
        return false;
    if (j > g_rows_ - 1)
        return false;
    return true;
}

bool Pathfinder::validCell(const int &i, const int &j) const
{
    if (!validIndex(i, j))
        return false;

    if (grid_[index(i, j)] == ZERO)
        return false;

    return true;
}

const int &Pathfinder::cellValue(const int &i, const int &j) const
{
    if (!validIndex(i, j))
        return ZERO;

    return grid_[index(i, j)];
}

Node *Pathfinder::newNode(const int &I, const int &J,
                          const float &G, const float &H, const float &F,
                          Node *Parent)
{
    Node *n = new Node(I, J, G, H, F, Parent);
    nodeList_.push_back(n);
    return n;
}

void Pathfinder::cleanUp()
{
    nodesUsed_ = nodeList_.size();
    for (auto &node : nodeList_)
        delete node;
    nodeList_.clear();
}

bool Pathfinder::validAdjacent_(const int &i, const int &j,
                                const int &center_i, const int &center_j,
                                const bool &diagonal) const
{
    if (!(abs(i) || abs(j)))
        return false;

    if (!validCell(i + center_i, j + center_j))
        return false;

    if ((abs(i) != abs(j)))
    {
        return true;
    }

    if (!diagonal)
    {
        return false;
    }

    int adj_i = i + (0 - i) + center_i;
    int adj_j = j + center_j;
    if (!validCell(adj_i, adj_j))
    {

        return false;
    }

    adj_i = i + center_i;
    adj_j = j + (0 - j) + center_j;
    if (!validCell(adj_i, adj_j))
    {

        return false;
    }

    return true;
}