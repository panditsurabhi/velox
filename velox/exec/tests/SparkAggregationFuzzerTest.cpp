/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <folly/init/Init.h>
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <unordered_set>

#include "velox/exec/tests/utils/AggregationFuzzerRunner.h"
#include "velox/exec/tests/utils/DuckQueryRunner.h"
#include "velox/functions/sparksql/aggregates/Register.h"

DEFINE_int64(
    seed,
    0,
    "Initial seed for random number generator used to reproduce previous "
    "results (0 means start with random seed).");

DEFINE_string(
    only,
    "",
    "If specified, Fuzzer will only choose functions from "
    "this comma separated list of function names "
    "(e.g: --only \"min\" or --only \"sum,avg\").");

int main(int argc, char** argv) {
  facebook::velox::functions::aggregate::sparksql::registerAggregateFunctions(
      "");

  ::testing::InitGoogleTest(&argc, argv);

  // Calls common init functions in the necessary order, initializing
  // singletons, installing proper signal handlers for better debugging
  // experience, and initialize glog and gflags.
  folly::init(&argc, &argv);

  // TODO: List of the functions that at some point crash or fail and need to
  // be fixed before we can enable. Constant argument of bloom_filter_agg cause
  // fuzzer test fail.
  std::unordered_set<std::string> skipFunctions = {"bloom_filter_agg"};

  // The results of the following functions depend on the order of input
  // rows. For some functions, the result can be transformed to a value that
  // doesn't depend on the order of inputs. If such transformation exists, it
  // can be specified to be used for results verification. If no transformation
  // is specified, results are not verified.
  std::unordered_map<std::string, std::string> customVerificationFunctions = {
      {"last", ""},
      {"last_ignore_null", ""},
      {"first", ""},
      {"first_ignore_null", ""},
      // TODO: Skip result verification of companion functions that return
      // complex types that contain floating-point fields for now, until we
      // fix test utilities in QueryAssertions to tolerate floating-point
      // imprecision in complex types.
      // https://github.com/facebookincubator/velox/issues/4481
      {"avg_partial", ""},
      {"avg_merge", ""},
      {"max_by", ""},
      {"min_by", ""}};

  size_t initialSeed = FLAGS_seed == 0 ? std::time(nullptr) : FLAGS_seed;
  auto duckQueryRunner =
      std::make_unique<facebook::velox::exec::test::DuckQueryRunner>();
  return facebook::velox::exec::test::AggregationFuzzerRunner::runFuzzer(
      FLAGS_only,
      initialSeed,
      std::nullopt,
      std::move(duckQueryRunner),
      skipFunctions,
      customVerificationFunctions);
}
