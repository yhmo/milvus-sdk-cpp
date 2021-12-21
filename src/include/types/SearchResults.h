// Licensed to the LF AI & Data foundation under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership. The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <string>
#include <vector>

#include "FieldData.h"

namespace milvus {

struct IDScorePair {
    int64_t id_ = 0;
    float score_ = 0.0;
};

struct SingleResult {
    std::vector<IDScorePair> topk_items_;
    std::vector<FieldDataPtr> output_fields_;
};

/**
 * @brief Results for Search().
 */
class SearchResults {
 public:
    explicit SearchResults(std::vector<SingleResult>& results) {
        nq_results_.swap(results);
    }

    /**
     * @brief Get search results.
     */
    std::vector<SingleResult>&
    Results() {
        return nq_results_;
    }

 private:
    std::vector<SingleResult> nq_results_;
};

}  // namespace milvus
