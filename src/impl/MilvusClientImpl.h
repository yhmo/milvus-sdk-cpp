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

#include <memory>

#include "MilvusClient.h"
#include "MilvusConnection.h"

/**
 *  @brief namespace milvus
 */
namespace milvus {

/**
 * @brief milvus client implementation
 */
class MilvusClientImpl : public MilvusClient {
 public:
    MilvusClientImpl() = default;
    virtual ~MilvusClientImpl();

    Status
    Connect(const ConnectParam& connect_param) final;

    Status
    Disconnect() final;

    Status
    CreateCollection(const CollectionSchema& schema) final;

    Status
    HasCollection(const std::string& collection_name, bool& has) final;

    Status
    DropCollection(const std::string& collection_name) final;

    Status
    LoadCollection(const std::string& collection_name, const TimeoutSetting* timeout) final;

    Status
    ReleaseCollection(const std::string& collection_name) final;

    Status
    DescribeCollection(const std::string& collection_name, CollectionDesc& collection_desc) final;

    Status
    GetCollectionStatistics(const std::string& collection_name, const TimeoutSetting* timeout,
                            CollectionStat& collection_stat) final;

    Status
    ShowCollections(const std::vector<std::string>& collection_names, CollectionsInfo& collections_info) final;

    Status
    CreatePartition(const std::string& collection_name, const std::string& partition_name) final;

    Status
    DropPartition(const std::string& collection_name, const std::string& partition_name) final;

    Status
    HasPartition(const std::string& collection_name, const std::string& partition_name, bool& has) final;

    Status
    LoadPartitions(const std::string& collection_name, const std::vector<std::string>& partition_names,
                   const TimeoutSetting* timeout) final;

    Status
    ReleasePartitions(const std::string& collection_name, const std::vector<std::string>& partition_names) final;

    Status
    GetPartitionStatistics(const std::string& collection_name, const std::string& partition_name,
                           const TimeoutSetting* timeout, PartitionStat& partition_stat) final;

    Status
    ShowPartitions(const std::string& collection_name, const std::vector<std::string>& partition_names,
                   PartitionsInfo& partitions_info) final;

 private:
    std::shared_ptr<MilvusConnection> connection_;
};

}  // namespace milvus
