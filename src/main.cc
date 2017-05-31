# include <iostream>
# include "include/path.hh"

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    std::cerr << "no input file" << std::endl;
    return 1;
  }
  Map map;
  map.create_map(argv[1]);
 map.print_path();
  map.find_path();
  map.print_path();
  map.print_map();
  return 0;
}
