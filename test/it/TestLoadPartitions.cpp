#include <gtest/gtest.h>

#include <chrono>

#include "mocks/MilvusMockedTest.h"

using ::milvus::StatusCode;
using ::milvus::proto::milvus::LoadPartitionsRequest;
using ::milvus::proto::milvus::ShowPartitionsRequest;
using ::milvus::proto::milvus::ShowPartitionsResponse;
using ::milvus::proto::milvus::ShowType;
using ::testing::_;
using ::testing::ElementsAreArray;
using ::testing::Property;

TEST_F(MilvusMockedTest, LoadPartitionsInstantly) {
    milvus::ConnectParam connect_param{"127.0.0.1", server_.ListenPort()};
    client_->Connect(connect_param);

    const std::string collection{"Foo"};
    const std::vector<std::string> partitions{"part1", "part2"};
    const auto timeout = ::milvus::TimeoutSetting::Instantly();

    EXPECT_CALL(service_,
                LoadPartitions(_,
                               AllOf(Property(&LoadPartitionsRequest::collection_name, collection),

                                     Property(&LoadPartitionsRequest::partition_names_size, partitions.size())),
                               _))
        .WillOnce([](::grpc::ServerContext*, const LoadPartitionsRequest*, milvus::proto::common::Status* response) {
            return ::grpc::Status{};
        });

    auto status = client_->LoadPartitions(collection, partitions, timeout);

    EXPECT_TRUE(status.IsOk());
}

TEST_F(MilvusMockedTest, LoadPartitionsFailure) {
    milvus::ConnectParam connect_param{"127.0.0.1", server_.ListenPort()};
    client_->Connect(connect_param);

    const std::string collection{"Foo"};
    const std::vector<std::string> partitions{"part1", "part2"};
    const ::milvus::TimeoutSetting timeout{5};

    EXPECT_CALL(service_,
                LoadPartitions(_,
                               AllOf(Property(&LoadPartitionsRequest::collection_name, collection),

                                     Property(&LoadPartitionsRequest::partition_names_size, partitions.size())),
                               _))
        .WillOnce([](::grpc::ServerContext*, const LoadPartitionsRequest*, milvus::proto::common::Status* response) {
            response->set_error_code(::milvus::proto::common::ErrorCode::UnexpectedError);
            return ::grpc::Status{};
        });

    auto status = client_->LoadPartitions(collection, partitions, timeout);

    EXPECT_FALSE(status.IsOk());
    EXPECT_EQ(status.Code(), StatusCode::SERVER_FAILED);
}

TEST_F(MilvusMockedTest, LoadPartitionsWithQueryStatusSuccess) {
    milvus::ConnectParam connect_param{"127.0.0.1", server_.ListenPort()};
    client_->Connect(connect_param);

    const std::string collection{"Foo"};
    const std::vector<std::string> partitions{"part1", "part2"};
    ::milvus::TimeoutSetting timeout{10};
    timeout.SetInterval(1);

    EXPECT_CALL(service_,
                LoadPartitions(_,
                               AllOf(Property(&LoadPartitionsRequest::collection_name, collection),

                                     Property(&LoadPartitionsRequest::partition_names_size, partitions.size())),
                               _))
        .WillOnce([](::grpc::ServerContext*, const LoadPartitionsRequest*, milvus::proto::common::Status*) {
            return ::grpc::Status{};
        });

    int show_partitions_called = 0;
    EXPECT_CALL(service_,
                ShowPartitions(_,
                               AllOf(Property(&ShowPartitionsRequest::collection_name, collection),

                                     Property(&ShowPartitionsRequest::partition_names_size, partitions.size())),
                               _))
        .Times(10)
        .WillRepeatedly([&show_partitions_called, &partitions](::grpc::ServerContext*, const ShowPartitionsRequest*,
                                                               ShowPartitionsResponse* response) {
            ++show_partitions_called;
            for (const auto& partition : partitions) {
                response->add_partition_names(partition);
                response->add_partitionids(0);
                response->add_created_timestamps(0);
                response->add_inmemory_percentages(10 * show_partitions_called);
            }
            return ::grpc::Status{};
        });

    auto status = client_->LoadPartitions(collection, partitions, timeout);

    EXPECT_TRUE(status.IsOk());
}

TEST_F(MilvusMockedTest, LoadPartitionsWithQueryStatusOomFailure) {
    milvus::ConnectParam connect_param{"127.0.0.1", server_.ListenPort()};
    client_->Connect(connect_param);

    const std::string collection{"Foo"};
    const std::vector<std::string> partitions{"part1", "part2"};
    ::milvus::TimeoutSetting timeout{10};
    timeout.SetInterval(1);

    EXPECT_CALL(service_,
                LoadPartitions(_,
                               AllOf(Property(&LoadPartitionsRequest::collection_name, collection),

                                     Property(&LoadPartitionsRequest::partition_names_size, partitions.size())),
                               _))
        .WillOnce([](::grpc::ServerContext*, const LoadPartitionsRequest*, milvus::proto::common::Status*) {
            return ::grpc::Status{};
        });

    EXPECT_CALL(service_,
                ShowPartitions(_,
                               AllOf(Property(&ShowPartitionsRequest::collection_name, collection),

                                     Property(&ShowPartitionsRequest::partition_names_size, partitions.size())),
                               _))
        .Times(1)
        .WillRepeatedly(
            [&partitions](::grpc::ServerContext*, const ShowPartitionsRequest*, ShowPartitionsResponse* response) {
                for (const auto& partition : partitions) {
                    response->add_partition_names(partition);
                    response->add_partitionids(0);
                    response->add_created_timestamps(0);
                    response->add_inmemory_percentages(10);
                }
                response->mutable_status()->set_error_code(::milvus::proto::common::ErrorCode::OutOfMemory);
                return ::grpc::Status{};
            });

    auto status = client_->LoadPartitions(collection, partitions, timeout);

    EXPECT_FALSE(status.IsOk());
    EXPECT_EQ(status.Code(), StatusCode::SERVER_FAILED);
}

TEST_F(MilvusMockedTest, LoadPartitionsWithQueryStatusTimeout) {
    milvus::ConnectParam connect_param{"127.0.0.1", server_.ListenPort()};
    client_->Connect(connect_param);

    const std::string collection{"Foo"};
    const std::vector<std::string> partitions{"part1", "part2"};
    ::milvus::TimeoutSetting timeout{1};
    timeout.SetInterval(110);

    EXPECT_CALL(service_,
                LoadPartitions(_,
                               AllOf(Property(&LoadPartitionsRequest::collection_name, collection),

                                     Property(&LoadPartitionsRequest::partition_names_size, partitions.size())),
                               _))
        .WillOnce([](::grpc::ServerContext*, const LoadPartitionsRequest*, milvus::proto::common::Status*) {
            return ::grpc::Status{};
        });

    EXPECT_CALL(service_,
                ShowPartitions(_,
                               AllOf(Property(&ShowPartitionsRequest::collection_name, collection),

                                     Property(&ShowPartitionsRequest::partition_names_size, partitions.size())),
                               _))
        .Times(10)
        .WillRepeatedly(
            [&partitions](::grpc::ServerContext*, const ShowPartitionsRequest*, ShowPartitionsResponse* response) {
                for (const auto& partition : partitions) {
                    response->add_partition_names(partition);
                    response->add_partitionids(0);
                    response->add_created_timestamps(0);
                    response->add_inmemory_percentages(0);
                }
                return ::grpc::Status{};
            });

    auto started = std::chrono::steady_clock::now();
    auto status = client_->LoadPartitions(collection, partitions, timeout);
    auto finished = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(finished - started).count();

    EXPECT_FALSE(status.IsOk());
    EXPECT_EQ(status.Code(), StatusCode::TIMEOUT);
}
