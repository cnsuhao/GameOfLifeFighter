#ifndef GRID_H
#define GRID_H

class Grid
{
    public:
    Grid(const int x, const int y);
    void Set(const int x, const int y, const int i);
    int Get(const int x, const int y);
    void Next();

    private:
    //int coordinate[int x_size][int y_size][int value_size];
};

#endif // GRID_H
