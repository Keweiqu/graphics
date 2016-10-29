#include "meshManager.hpp"
#include "util.hpp"

int main(int argc, char* argv[]) {
  meshManager mesh;
  mesh.readFiles(argc - 1, argv + 1);

  for(unsigned int i = 0; i < mesh.draw_sequence.size(); i++) {
    string filename = mesh.draw_sequence[i];
    metadata md = (*mesh.filename_metadata)[filename];
    cout << "Printing data for " << filename << endl;
    cout << "num vertices: " << md.num_of_vertices << " num indices: " << md.num_of_indices << endl;
    cout << "vn_offset: " << md.vn_offset << " indices_offset: " << md.indices_offset << endl;
  }

  cout << "Offsets: ";
  cout << "vn offset: " << mesh.vn_offset << ", " << "idx_offset: " << mesh.idx_offset << ", " << "flat offset: " << mesh.flat_offset << endl;

  cout << "Flat vertices: ";
  for (unsigned int i = 0; i < mesh.flat_vertices->size(); i += 3) {
    cout << mesh.flat_vertices->at(i) << " " << mesh.flat_vertices->at(i + 1) << " " << mesh.flat_vertices->at(i + 2) << ", ";
  }
  cout << endl;

  cout << "Flat normals: " << endl;
  for (unsigned int i = 0; i < mesh.flat_normals->size(); i += 3) {
    cout << "x: " << mesh.flat_normals->at(i) << ", " << "y: " << mesh.flat_normals->at(i + 1) << ", " << "z: " << mesh.flat_normals->at(i + 2) << endl;
  }
  cout << endl;
  return 0;
}
