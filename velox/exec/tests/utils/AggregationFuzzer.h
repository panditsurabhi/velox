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
#pragma once

#include "velox/exec/Aggregate.h"
#include "velox/exec/tests/utils/ReferenceQueryRunner.h"

namespace facebook::velox::exec::test {

static constexpr const std::string_view kPlanNodeFileName = "plan_nodes";

/// Runs the aggregation fuzzer.
/// @param signatureMap Map of all aggregate function signatures.
/// @param seed Random seed - Pass the same seed for reproducibility.
/// @param orderDependentFunctions Map of functions that depend on order of
/// input.
/// @param planPath Path to persisted plan information. If this is
/// supplied, fuzzer will only verify the plans.
/// @param referenceQueryRunner Reference query runner for results
/// verification.
void aggregateFuzzer(
    AggregateFunctionSignatureMap signatureMap,
    size_t seed,
    const std::unordered_map<std::string, std::string>& orderDependentFunctions,
    const std::optional<std::string>& planPath,
    std::unique_ptr<ReferenceQueryRunner> referenceQueryRunner);
} // namespace facebook::velox::exec::test
