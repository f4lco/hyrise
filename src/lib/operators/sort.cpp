#include "sort.hpp"

#include <memory>
#include <string>

namespace opossum {
Sort::Sort(const std::shared_ptr<const AbstractOperator> in, const std::string &sort_column_name, const bool ascending)
    : AbstractOperator(in),
      _impl(make_unique_by_column_type<AbstractOperatorImpl, SortImpl>(
          _input_left->column_type(_input_left->column_id_by_name(sort_column_name)), in, sort_column_name,
          ascending)) {}
const std::string Sort::name() const { return "Sort"; }

uint8_t Sort::num_in_tables() const { return 1; }

uint8_t Sort::num_out_tables() const { return 1; }

void Sort::execute() { _impl->execute(); }

std::shared_ptr<const Table> Sort::get_output() const { return _impl->get_output(); }

}  // namespace opossum