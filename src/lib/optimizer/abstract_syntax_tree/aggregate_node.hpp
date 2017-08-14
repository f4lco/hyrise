#pragma once

#include <memory>
#include <string>
#include <vector>

#include "optimizer/abstract_syntax_tree/abstract_ast_node.hpp"

#include "common.hpp"
#include "types.hpp"

namespace opossum {

class ExpressionNode;

struct AggregateColumnDefinition {
  explicit AggregateColumnDefinition(const std::shared_ptr<ExpressionNode>& expr,
                                     const optional<std::string>& alias = {});

  std::shared_ptr<ExpressionNode> expr;
  optional<std::string> alias;
};

/**
 * This node type is used to describe SELECT lists for statements that have at least one of the following:
 *  - one or more aggregate functions in their SELECT list
 *  - a GROUP BY clause
 */
class AggregateNode : public AbstractASTNode {
 public:
  explicit AggregateNode(const std::vector<AggregateColumnDefinition>& aggregates,
                         const std::vector<ColumnID>& groupby_columns);

  const std::vector<AggregateColumnDefinition>& aggregates() const;

  const std::vector<ColumnID>& groupby_columns() const;

  std::string description() const override;

  std::vector<ColumnID> output_column_ids() const override;
  bool find_column_id_for_column_name(std::string & column_name, ColumnID &column_id) override;

 private:
  std::vector<AggregateColumnDefinition> _aggregates;
  std::vector<ColumnID> _groupby_columns;
};

}  // namespace opossum
