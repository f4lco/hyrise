#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../base_test.hpp"
#include "gtest/gtest.h"

#include "operators/pqp_expression.hpp"
#include "operators/projection.hpp"
#include "operators/table_wrapper.hpp"
#include "operators/union_all.hpp"
#include "storage/table.hpp"
#include "types.hpp"

namespace opossum {
class OperatorsUnionAllTest : public BaseTest {
 protected:
  virtual void SetUp() {
    _table_wrapper_a = std::make_shared<TableWrapper>(load_table("src/test/tables/int_float.tbl", 2));

    _table_wrapper_b = std::make_shared<TableWrapper>(load_table("src/test/tables/int_float2.tbl", 2));

    _table_wrapper_a->execute();
    _table_wrapper_b->execute();
  }

  std::shared_ptr<TableWrapper> _table_wrapper_a;
  std::shared_ptr<TableWrapper> _table_wrapper_b;
};

TEST_F(OperatorsUnionAllTest, UnionOfValueTables) {
  std::shared_ptr<Table> expected_result = load_table("src/test/tables/int_float_union.tbl", 2);

  auto union_all = std::make_shared<UnionAll>(_table_wrapper_a, _table_wrapper_b);
  union_all->execute();

  EXPECT_TABLE_EQ_UNORDERED(union_all->get_output(), expected_result);
}

TEST_F(OperatorsUnionAllTest, UnionOfValueReferenceTables) {
  std::shared_ptr<Table> expected_result = load_table("src/test/tables/int_float_union.tbl", 2);

  auto projection = std::make_shared<Projection>(
      _table_wrapper_a, Projection::ColumnExpressions{PQPExpression::create_column(ColumnID{0}),
                                                      PQPExpression::create_column(ColumnID{1})});
  projection->execute();

  auto union_all = std::make_shared<UnionAll>(projection, _table_wrapper_b);
  union_all->execute();

  EXPECT_TABLE_EQ_UNORDERED(union_all->get_output(), expected_result);
}

TEST_F(OperatorsUnionAllTest, ThrowWrongColumnNumberException) {
  if (!IS_DEBUG) return;
  std::shared_ptr<Table> test_table_c = load_table("src/test/tables/int.tbl", 2);
  auto gt_c = std::make_shared<TableWrapper>(std::move(test_table_c));
  gt_c->execute();

  auto union_all = std::make_shared<UnionAll>(_table_wrapper_a, gt_c);

  EXPECT_THROW(union_all->execute(), std::exception);
}

TEST_F(OperatorsUnionAllTest, ThrowWrongColumnOrderException) {
  if (!IS_DEBUG) return;
  std::shared_ptr<Table> test_table_d = load_table("src/test/tables/float_int.tbl", 2);
  auto gt_d = std::make_shared<TableWrapper>(std::move(test_table_d));
  gt_d->execute();

  auto union_all = std::make_shared<UnionAll>(_table_wrapper_a, gt_d);

  EXPECT_THROW(union_all->execute(), std::exception);
}

}  // namespace opossum
