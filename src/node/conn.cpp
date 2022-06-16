#include "../../inc/node.hpp"

/** updates */
void ConnCtx::UpdateParentMap(std::map<std::string, Tree>* m) {
  this->GraphCtx->ParentMap = m;
}
void ConnCtx::UpdateTimeout(unsigned int t) {
  this->Networking.tout = t;
}

/** init */
ConnCtx::ConnCtx(
  std::map<std::string, Tree>* pm,
  Peer p
) : Networking(p), FlagManager(3) { 
  this->UpdateParentMap(pm);
}
