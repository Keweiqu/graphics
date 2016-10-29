#include "readFile.hpp"
#include "meshManager.hpp"
#include "util.hpp"

int main(int argc, char* argv[]) {
  meshManager mesh;
  mesh.readFiles(argc - 1, argv + 1);
  for(int i = 0; i < mesh.draw_sequence.size(); i++) {
    string filename = mesh.draw_sequence[i];
    metadata md = (*mesh.filename_metadata)[filename];
    cout << "Printing data for " << filename << endl;
    cout << "num vertices: " << md.num_of_vertices << " num indices: " << md.num_of_indices << endl;
    cout << "vn_offset: " << md.vn_offset << " indices_offset: " << md.indices_offset << endl;
  }
  return 0;
}
