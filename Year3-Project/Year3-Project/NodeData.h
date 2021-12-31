#ifndef YEAR3_PROJECT_NODEDATA_H
#define YEAR3_PROJECT_NODEDATA_H


class NodeData
{
private:
    int m_id;
    bool m_isPassable;

public:
    NodeData(int id, bool isPassable);

    int getId() const;

    void setPassable(bool passable);
    bool isPassable() const;
};


#endif //YEAR3_PROJECT_NODEDATA_H
