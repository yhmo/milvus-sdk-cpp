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

#include "Status.h"
#include "types/CollectionDesc.h"
#include "types/CollectionInfo.h"
#include "types/CollectionSchema.h"
#include "types/CollectionStat.h"
#include "types/ConnectParam.h"
#include "types/PartitionInfo.h"
#include "types/PartitionStat.h"
#include "types/TimeoutSetting.h"

/**
 *  @brief namespace milvus
 */
namespace milvus {

/**
 * @brief milvus client abstract class
 */
class MilvusClient {
 public:
    /**
     * Create a MilvusClient instance.
     *
     * @return std::shared_ptr<MilvusClient>
     */
    static std::shared_ptr<MilvusClient>
    Create();

    /**
     * Connect to Milvus server.
     *
     * @param [in] connect_param server address and port
     * @return Status operation successfully or not
     */
    virtual Status
    Connect(const ConnectParam& connect_param) = 0;

    /**
     * Break connections between client and server.
     *
     * @return Status operation successfully or not
     */
    virtual Status
    Disconnect() = 0;

    /**
     * Create a collection with schema.
     *
     * @param [in] schema schema of the collection
     * @return Status operation successfully or not
     */
    virtual Status
    CreateCollection(const CollectionSchema& schema) = 0;

    /**
     * Check existence of a collection.
     *
     * @param [in] collection_name name of the collection
     * @param [out] has true: collection exists, false: collection doesn't exist
     * @return Status operation successfully or not
     */
    virtual Status
    HasCollection(const std::string& collection_name, bool& has) = 0;

    /**
     * Drop a collection, with all its partitions, index and segments.
     *
     * @param [in] collection_name name of the collection
     * @return Status operation successfully or not
     */
    virtual Status
    DropCollection(const std::string& collection_name) = 0;

    /**
     * Load collection data into CPU memory of query node.
     * If the timeout is specified, this api will call ShowCollections() to check collection's loading state,
     * waiting until the collection completely loaded into query node.
     *
     * @param [in] collection_name name of the collection
     * @param [in] timeout timeout setting for loading, set to nullptr to return instantly
     * @return Status operation successfully or not
     */
    virtual Status
    LoadCollection(const std::string& collection_name, const TimeoutSetting* timeout) = 0;

    /**
     * Release collection data from query node.
     *
     * @param [in] collection_name name of the collection
     * @return Status operation successfully or not
     */
    virtual Status
    ReleaseCollection(const std::string& collection_name) = 0;

    /**
     * Get collection description, including its schema.
     *
     * @param [in] collection_name name of the collection
     * @param [out] collection_desc collection's description
     * @return Status operation successfully or not
     */
    virtual Status
    DescribeCollection(const std::string& collection_name, CollectionDesc& collection_desc) = 0;

    /**
     * Get collection statistics, currently only return row count.
     * If the timeout is specified, this api will call Flush() and wait all segmetns persisted into storage.
     *
     * @param [in] collection_name name of the collection
     * @param [in] timeout specify time out for sync-flush mode, set to nullptr to return instantly
     * @param [out] collection_stat statistics of the collection
     * @return Status operation successfully or not
     */
    virtual Status
    GetCollectionStatistics(const std::string& collection_name, const TimeoutSetting* timeout,
                            CollectionStat& collection_stat) = 0;

    /**
     * If the collection_names is empty, list all collections brief informations.
     * If the collection_names is specified, return the specified collection's loading process state.
     *
     * @param [in] collection_names name array of collections
     * @param [out] collections_info brief informations of the collections
     * @return Status operation successfully or not
     */
    virtual Status
    ShowCollections(const std::vector<std::string>& collection_names, CollectionsInfo& collections_info) = 0;

    /**
     * Create a partition in a collection.
     *
     * @param [in] collection_name name of the collection
     * @param [in] partition_name name of the partition
     * @return Status operation successfully or not
     */
    virtual Status
    CreatePartition(const std::string& collection_name, const std::string& partition_name) = 0;

    /**
     * Drop a partition, with its index and segments.
     *
     * @param [in] collection_name name of the collection
     * @param [in] partition_name name of the partition
     * @return Status operation successfully or not
     */
    virtual Status
    DropPartition(const std::string& collection_name, const std::string& partition_name) = 0;

    /**
     * Check existence of a partition.
     *
     * @param [in] collection_name name of the collection
     * @param [in] partition_name name of the partition
     * @param [out] has true: partition exists, false: partition doesn't exist
     * @return Status operation successfully or not
     */
    virtual Status
    HasPartition(const std::string& collection_name, const std::string& partition_name, bool& has) = 0;

    /**
     * Load specific partitions data of one collection into query nodes.
     * If the timeout is specified, this api will call ShowPartitions() to check partition's loading state,
     * waiting until the collection completely loaded into query node.
     *
     * @param [in] collection_name name of the collection
     * @param [in] partition_names name array of the partitions
     * @param [in] timeout timeout setting for loading, set to nullptr to return instantly
     * @return Status operation successfully or not
     */
    virtual Status
    LoadPartitions(const std::string& collection_name, const std::vector<std::string>& partition_names,
                   const TimeoutSetting* timeout) = 0;

    /**
     * Release specific partitions data of one collection into query nodes.
     *
     * @param [in] collection_name name of the collection
     * @param [in] partition_names name array of the partitions
     * @return Status operation successfully or not
     */
    virtual Status
    ReleasePartitions(const std::string& collection_name, const std::vector<std::string>& partition_names) = 0;

    /**
     * Get partition statistics, currently only return row count.
     * If the timeout is specified, this api will call Flush() and wait all segmetns persisted into storage.
     *
     * @param [in] collection_name name of the collection
     * @param [in] partition_name name of the partition
     * @param [in] timeout specify time out for sync-flush mode, set to nullptr to return instantly
     * @param [out] partition_stat statistics of the partition
     * @return Status operation successfully or not
     */
    virtual Status
    GetPartitionStatistics(const std::string& collection_name, const std::string& partition_name,
                           const TimeoutSetting* timeout, PartitionStat& partition_stat) = 0;

    /**
     * If the partition_names is empty, list all partitions brief informations.
     * If the partition_names is specified, return the specified partition's loading process state.
     *
     * @param [in] collection_name name of the collection
     * @param [in] partition_names name array of the partitions
     * @param [out] partitions_info brief informations of the partitions
     * @return Status operation successfully or not
     */
    virtual Status
    ShowPartitions(const std::string& collection_name, const std::vector<std::string>& partition_names,
                   PartitionsInfo& partitions_info) = 0;
};

}  // namespace milvus
