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
#include <unordered_map>

#include "Constants.h"

namespace milvus {

/**
 * @brief Partition statistics returned by GetPartitionStatistics().
 */
class PartitionStat {
 public:
    /**
     * @brief Return row count of this partition.
     *
     * @return uint64_t row count of this partition
     */
    uint64_t
    GetRowCount() const {
        const auto iter = statistics_.find(KEY_ROW_COUNT);
        if (iter == statistics_.end()) {
            // TODO: throw exception or log
            return 0;
        }

        std::string str = iter->second;

        return atol(str.c_str());
    }

 private:
    /**
     * @brief Name of this partition.
     */
    std::string name_;

    /**
     * @brief Partition statistics in key-value format.
     */
    std::unordered_map<std::string, std::string> statistics_;
};

}  // namespace milvus
