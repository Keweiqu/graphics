#include "readFile.hpp"
#include "meshManager.hpp"
#include "util.hpp"

int main(int argc, char* argv[]) {
  meshManager mesh;
  if (readFile(&mesh, argc, argv) == -1) {
    cout << "Exiting program..." << endl;
    return -1;
  }

  // cout << "Mesh vertice: ";
  // for (unsigned int i = 0; i < mesh.vertices->size(); i++) {
  //   cout << mesh.vertices->at(i) << " ";
  // }
  // cout << endl;

  // cout << "Mesh indices: ";
  // for (unsigned int i = 0; i < mesh.indices->size(); i++) {
  //   cout << mesh.indices->at(i) << " ";
  // }
  cout << endl;
  return 0;
}
