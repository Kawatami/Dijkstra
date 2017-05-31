# pragma once 
# include <iostream>
# include <vector>
# include <queue>
# include <functional>

enum floor
{
  FLOOR     =5,
  MUD       =8,
  CONCRETE  =2,
  WALL      =10000000000,
  START     =0,
  END       =1
};

typedef struct
{
  int x;
  int y;
 
}pos;

class Cellule
{
  public:
    void set_pos(int x, int y);
    void set_parent(Cellule* c);
    void set_cost(size_t n);
    void set_floor(enum floor f);
    void set_path(bool b);
    void set_mark(bool b);
    bool get_mark();
    bool get_path();
    enum floor get_floor();
    size_t get_cost();
    pos get_pos();
    Cellule* get_parent();
    bool operator >(Cellule c)
    {
      return (this->cost_ > c.get_cost());
    }
  private:
    size_t cost_;
    pos pos_;
    Cellule* parent_;
    enum floor floor_;
    bool path_;
    bool mark_;
};

class cmp
{
  public :
  bool operator()(Cellule* a, Cellule *b)
  {
    return a->get_cost() > b->get_cost();
  }
};

class Map
{
  public :
    void set_width(int n);
    void set_height(int n);
    void set_start(Cellule c);
    void set_end(Cellule c);
    Cellule get_start();
    Cellule get_end();
    int get_width();
    int get_height();
    void create_map(char *path);
    void find_path();
    void print_map();
    void print_path();
    bool test(int x, int y);
    bool cmp_cost(int x, int y, int px, int py, int f);
    void process(int x, int y, int cost, int t,
    std::priority_queue<Cellule*, std::vector<Cellule*>, cmp> &f);
  private :
    int width_;
    int height_;
    std::vector< std::vector<Cellule> > map_;
    Cellule start_;
    Cellule end_;
};

# include "path.hxx"
