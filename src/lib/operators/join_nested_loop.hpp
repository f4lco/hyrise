#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "abstract_join_operator.hpp"
#include "types.hpp"
#include "utils/assert.hpp"

namespace opossum {

class JoinNestedLoop : public AbstractJoinOperator {
 public:
  JoinNestedLoop(const std::shared_ptr<const AbstractOperator> left,
                 const std::shared_ptr<const AbstractOperator> right, const JoinMode mode,
                 const std::pair<ColumnID, ColumnID>& column_ids, const ScanType scan_type);

  const std::string name() const override;
  std::shared_ptr<AbstractOperator> recreate(const std::vector<AllParameterVariant>& args = {}) const override;

 protected:
  std::shared_ptr<const Table> _on_execute() override;

  void _perform_join();

  void _create_table_structure();

  void _write_output_chunks(Chunk& output_chunk, const std::shared_ptr<const Table> input_table,
                            std::shared_ptr<PosList> pos_list);

  std::shared_ptr<Table> _output_table;
  std::shared_ptr<const Table> _left_in_table;
  std::shared_ptr<const Table> _right_in_table;
  ColumnID _left_column_id;
  ColumnID _right_column_id;
};

}  // namespace opossum
