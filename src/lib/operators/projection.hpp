#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "abstract_read_only_operator.hpp"
#include "termfactory.hpp"
#include "types.hpp"

namespace opossum {

// operator to select a subset of the set of all columns found in the table
class Projection : public AbstractReadOnlyOperator {
 public:
  // Defines one output column
  // Expression that will be converted into a term composition
  // Type as String
  // Name of the new column
  using ProjectionDefinition = std::tuple<std::string /*Expression*/, std::string /*Type*/, std::string /*Name*/>;
  using ProjectionDefinitions = std::vector<ProjectionDefinition>;

  Projection(const std::shared_ptr<const AbstractOperator> in, const ProjectionDefinitions& definitions);
  Projection(const std::shared_ptr<const AbstractOperator> in, const std::vector<std::string>& columns);

  const std::string name() const override;
  uint8_t num_in_tables() const override;
  uint8_t num_out_tables() const override;

 protected:
  ProjectionDefinitions _projection_definitions;
  std::vector<std::string> _simple_projection;

  class ColumnCreator {
   public:
    template <typename T>
    static void run(Chunk& chunk, const ChunkID chunk_id, const std::string& expression,
                    std::shared_ptr<const Table> input_table_left) {
      auto term = TermFactory<T>::build_term(expression);

      // check whether term is a just a simple column and bypass this column
      if (auto variable_term = std::dynamic_pointer_cast<VariableTerm<T>>(term)) {
        auto bypassed_column = variable_term->get_column(input_table_left, chunk_id);
        return chunk.add_column(bypassed_column);
      }

      auto values = term->get_values(input_table_left, chunk_id);
      auto column = std::make_shared<ValueColumn<T>>(std::move(values));

      chunk.add_column(column);
    }
  };
  std::shared_ptr<const Table> on_execute() override;
};
}  // namespace opossum
