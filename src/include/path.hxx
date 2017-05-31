# pragma once
# include "path.hh"
# include <fstream>
# include <array>

# define NRM  "\x1B[0m"
# define RED  "\x1B[31m"
# define GRN  "\x1B[32m"
# define YEL  "\x1B[33m"
# define BLU  "\x1B[34m"
# define MAG  "\x1B[35m"
# define CYN  "\x1B[36m"
# define WHT  "\x1B[37m"

/* Cellule */

void Cellule::set_pos(int x, int y)
{
  this->pos_.x = x;
  this->pos_.y = y;
}

void Cellule::set_parent(Cellule* c)
{
  this->parent_ = c;
}

void Cellule::set_cost(size_t n)
{
  this->cost_ = n;
}

void Cellule::set_floor(enum floor f)
{
  this->floor_ = f;
}

enum floor Cellule::get_floor()
{
  return this->floor_;
}

void Cellule::set_path(bool b)
{
  this->path_ = b;
}

void Cellule::set_mark(bool b)
{
  this->mark_ = b;
}

bool Cellule::get_path()
{
  return this->path_;
}

bool Cellule::get_mark()
{
  return this->mark_;
}

size_t Cellule::get_cost()
{
  return this->cost_;
}


pos Cellule::get_pos()
{
  return this->pos_;
}

Cellule* Cellule::get_parent()
{
  return this->parent_;
}

/* Map */

void Map::set_width(int n)
{
  this->width_ = n;
}

void Map::set_height(int n)
{
  this->height_ = n;
}

void Map::set_start(Cellule c)
{
  this->start_ = c;
}

void Map::set_end(Cellule c)
{
  this->end_ = c;
}

Cellule Map::get_start()
{
  return this->start_;
}

Cellule Map::get_end()
{
  return this->end_;
}

int Map::get_width()
{
  return this->width_;
}

int Map::get_height()
{
  return this->height_;
}

void Map::create_map(char *path)
{
  this->start_.set_pos(-1, -1);
  this->end_.set_pos(-1, -1);
  std::fstream file;
  std::string token;
  file.open(path);
  size_t x = 0;
  size_t y = 0;
  while (file.good())
  {
    std::vector<Cellule> vect;
    if (getline(file, token))
    {
      for (auto current : token)
      {
        Cellule c;
        c.set_cost(1000000000);
        c.set_path(false);
        c.set_pos(y, x);
        c.set_mark(false);
        c.set_parent(NULL);
        if (current == 'F')
          c.set_floor(FLOOR);
        else if (current == 'W')
          c.set_floor(WALL);
        else if (current == 'M')
          c.set_floor(MUD);
        else if (current == 'C')
          c.set_floor(CONCRETE);
        else if (current == 'S')
        {
          c.set_cost(0);
          c.set_floor(START);
          this->start_ = c;
        }
        else if (current == 'E')
        {
          c.set_floor(END);
          this->end_ = c;
        }
        else
        {
          std::cerr << "ligne : " << y << " column : " << x 
            << " unknown floor type \"" << current 
            << "\"" << std::endl;
          exit(1);
        }
        vect.push_back(c);
        x++;
        this->width_ = y;
        this->height_ = x;
      }
      this->map_.push_back(vect);
      x = 0;
      y++; 
    }
  }
  if (this->start_.get_pos().x == -1 || 
      this->end_.get_pos().x == -1)
  {
    std::cerr << "Missing start point or end point" << std::endl;
    exit(1);
  }
}

bool Map::test(int x, int y)
{
  return this->map_[x][y].get_floor() != WALL && 0 <= x &&
    x < this->width_ && 0 <= y && y < this->height_
    && !this->map_[x][y].get_mark();
}

bool Map::cmp_cost(int x, int y, int px,int py, int f) // px/py current cell
{                                                      // x/y next cell
  return this->map_[x][y].get_cost()                   // f next cell's cost
    > this->map_[px][py].get_cost() + f;
}

void Map::process(int x, int y, int cost, int t,
 std::priority_queue<Cellule*, std::vector<Cellule*>,cmp > &f)
{
      this->map_[x][y].set_cost(cost + t);
      this->map_[x][y].set_parent(&this->map_[x][y]);
      f.push(&this->map_[x][y]);
}

void Map::find_path()
{
  int x_end = this->end_.get_pos().x;
  int y_end = this->end_.get_pos().y;
  bool flag_find = false;
  std::priority_queue<Cellule*, std::vector<Cellule*>,cmp > f;
  f.push(&this->map_[this->start_.get_pos().x][this->start_.get_pos().y]);
  while (!f.empty())
  {
    Cellule* current = f.top();
    f.pop();
    int x = current->get_pos().x;
    int y = current->get_pos().y;
    int cost = this->map_[x][y].get_cost();
    if (x_end == x && y_end == y)
    {
      flag_find = true;
      break;
    }
    current->set_mark(true);
    /* UP */ 
    if (test( x - 1, y) && cmp_cost(x - 1, y, x, y, this->map_[x - 1][y].get_floor() ))
    {
      this->map_[x - 1][y].set_cost(cost + this->map_[x - 1][y].get_floor());
      this->map_[x - 1][y].set_parent(&this->map_[x][y]);
      f.push(&this->map_[x - 1][y]);
    }

    /* DOWN */ 
    if (test(x + 1, y) && cmp_cost(x + 1, y, x, y, this->map_[x + 1][y].get_floor()))
    {        
      this->map_[x + 1][y].set_cost(cost + this->map_[x + 1][y].get_floor());
      this->map_[x + 1][y].set_parent(&this->map_[x][y]);
      f.push(&this->map_[x + 1][y]);
    }

    /* LEFT */ 
    if (test(x, y - 1) && cmp_cost(x, y - 1, x, y, this->map_[x][y - 1].get_floor()))
    {
      this->map_[x][y - 1].set_cost(cost + this->map_[x][y - 1].get_floor());
      this->map_[x][y - 1].set_parent(&this->map_[x][y]);
      f.push(&this->map_[x][y - 1]);
    }

    /* RIGHT */ 
    if (test(x, y + 1) && cmp_cost(x, y + 1, x, y, this->map_[x][y + 1].get_floor()))
    {
      this->map_[x][y + 1].set_cost(cost + this->map_[x][y + 1].get_floor());
      this->map_[x][y + 1].set_parent(&this->map_[x][y]);
      f.push(&this->map_[x][y + 1]);
    }
  }
  if (!flag_find)
  {
    std::cerr << "Exit point unreachable\n";
    exit(1);
  }
  pos pos = this->end_.get_pos();
  Cellule *tmp = &this->map_[pos.x][pos.y];
  while (tmp)
  {
    tmp->set_path(true);
    tmp = tmp->get_parent();
  }
}

void Map::print_map()
{
int cost = 0; 
  for ( auto x : this->map_)
  {
    for (auto y : x)
    {
      if (y.get_floor() == START)
      {
        std::cout << BLU << "S";
      }
      else if (y.get_floor() == END)
      {
        std::cout << BLU << "E";
        cost = y.get_cost();
      }
      else if (y.get_path())
      {
        std::cout << RED << "X";
      }
      else if (y.get_mark())
      {
        std::cout << YEL << "X";
      }

      else if (y.get_floor() == WALL)
      {
        std::cout << WHT << "X" ;
      }
      else if (y.get_floor() == FLOOR)
      {
        std::cout << GRN << "X";
      }
      else if (y.get_floor() == MUD)
      {
        std::cout << YEL << "X";
      }
      else if (y.get_floor() == CONCRETE)
      {
        std::cout << CYN << "X";
      }
    }
    std::cout << std::endl;
  }
  std::cout << "Starting point : (" 
    << this->start_.get_pos().x
    << ", " << this->start_.get_pos().y 
    << ")" << std::endl;
  std::cout << "End point : (" 
    << this->end_.get_pos().x
    << ", " << this->end_.get_pos().y << ")" 
    << std::endl;
  std::cout << "Cost : " << cost << std::endl;
  std::cout << std::endl;

}

void Map::print_path()
{
 int cost = 0; 
  for ( auto x : this->map_)
  {
    for (auto y : x)
    {
      if (y.get_floor() == START)
      {
        std::cout << BLU << "S";
      }
      else if (y.get_floor() == END)
      {
        std::cout << BLU << "E";
        cost = y.get_cost();
      }
      else if (y.get_path())
      {
        std::cout << RED << "X";
      }
      else if (y.get_floor() == WALL)
      {
        std::cout << WHT << "X" ;
      }
      else if (y.get_floor() == FLOOR)
      {
        std::cout << GRN << "X";
      }
      else if (y.get_floor() == MUD)
      {
        std::cout << YEL << "X";
      }
      else if (y.get_floor() == CONCRETE)
      {
        std::cout << CYN << "X";
      }

    }
    std::cout << std::endl;
  }
  std::cout << "Starting point : (" 
    << this->start_.get_pos().x
    << ", " << this->start_.get_pos().y 
    << ")" << std::endl;
  std::cout << "End point : (" 
    << this->end_.get_pos().x
    << ", " << this->end_.get_pos().y << ")" 
    << std::endl;
  std::cout << "Cost : " << cost << std::endl;
  std::cout << std::endl;

}
/* 
   size_t width_;
   size_t height_;
   vector<vector<Cellule>> map_;
 */
