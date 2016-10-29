#include "meshManager.hpp"
#include "util.hpp"

int main(int argc, char* argv[]) {
  meshManager mesh;
  mesh.readFiles(argc - 1, argv + 1);

  cout << "Draw Sequence: ";
  for (unsigned int i = 0; i < mesh.draw_sequence->size(); i++) {
    cout << mesh.draw_sequence->at(i) << " ";
  }
  cout << endl;

  cout << "Offsets: ";
  cout << "vn offset: " << mesh.vn_offset << ", " << "idx_offset: " << mesh.idx_offset << ", " << "flat offset: " << mesh.flat_offset << endl;

  cout << "Flat vertices and normals: ";
  for (unsigned int i = 0; i < mesh.flat_vertices->size(); i++) {
    cout << mesh.flat_vertices->at(i) << " ";
  }
  cout << endl;
  // int i = (*mesh.filename_metadata)["extralarge/dragon.off"].num_of_indices;
  int i = (*mesh.filename_metadata)["tetra.off"].num_of_indices;

  // cout << "Mesh vertice: ";
  // for (unsigned int i = 0; i < mesh.vertices->size(); i++) {
  //   cout << mesh.vertices->at(i) << " ";
  // }
  // cout << endl;

  // cout << "Mesh indices: ";
  // for (unsigned int i = 0; i < mesh.indices->size(); i++) {
  //   cout << mesh.indices->at(i) << " ";
  // }
  // cout << i;
  return 0;
}
