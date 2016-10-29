#include "readFile.hpp"
#include "meshManager.hpp"
#include "util.hpp"

int main(int argc, char* argv[]) {
  meshManager mesh;
  mesh.readFiles(argc - 1, argv + 1);
  int i = (*mesh.filename_metadata)["extralarge/dragon.off"].num_of_indices;

  // cout << "Mesh vertice: ";
  // for (unsigned int i = 0; i < mesh.vertices->size(); i++) {
  //   cout << mesh.vertices->at(i) << " ";
  // }
  // cout << endl;

  // cout << "Mesh indices: ";
  // for (unsigned int i = 0; i < mesh.indices->size(); i++) {
  //   cout << mesh.indices->at(i) << " ";
  // }
  cout << i;
  return 0;
}
