#ifndef IMSQLITE_MODELS_DG_DB_NODE_HPP
#define IMSQLITE_MODELS_DG_DB_NODE_HPP

#include "imsqlite/models/db.hpp"
#include "node.hpp"

namespace imsql::models::dg {

class DbNode : public Node {
public:
  explicit DbNode(const models::Db& dbModel) : dbModel_(&dbModel) {}

protected:
  const models::Db* dbModel_;
};

} // namespace imsql::models::dg

#endif // IMSQLITE_MODELS_DG_DB_NODE_HPP
