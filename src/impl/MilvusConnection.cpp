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

#include "MilvusConnection.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

namespace milvus {
MilvusConnection::~MilvusConnection() {
    Disconnect();
}

Status
MilvusConnection::Connect(const std::string& uri) {
    ::grpc::ChannelArguments args;
    args.SetMaxSendMessageSize(-1);     // max send message size: 2GB
    args.SetMaxReceiveMessageSize(-1);  // max receive message size: 2GB
    channel_ = ::grpc::CreateCustomChannel(uri, ::grpc::InsecureChannelCredentials(), args);
    if (channel_ != nullptr) {
        stub_ = proto::milvus::MilvusService::NewStub(channel_);
        return Status::OK();
    }

    std::string reason = "Failed to connect uri: " + uri;
    return Status(StatusCode::NotConnected, reason);
}

Status
MilvusConnection::Disconnect() {
    stub_.release();
    channel_.reset();
    return Status::OK();
}

Status
MilvusConnection::CreateCollection(const proto::milvus::CreateCollectionRequest& request,
                                   proto::common::Status& response) {
    if (stub_ == nullptr) {
        return Status(StatusCode::NotConnected, "Connection is not ready!");
    }

    ClientContext context;
    ::grpc::Status grpc_status = stub_->CreateCollection(&context, request, &response);

    if (!grpc_status.ok()) {
        std::cerr << "CreateCollection failed!" << std::endl;
        return Status(StatusCode::ServerFailed, grpc_status.error_message());
    }

    return Status::OK();
}

Status
MilvusConnection::DropCollection(const proto::milvus::DropCollectionRequest& request, proto::common::Status& response) {
    return Status::OK();
}

Status
MilvusConnection::HasCollection(const proto::milvus::HasCollectionRequest& request,
                                proto::milvus::BoolResponse& response) {
    return Status::OK();
}

Status
MilvusConnection::LoadCollection(const proto::milvus::LoadCollectionRequest& request, proto::common::Status& response) {
    return Status::OK();
}

Status
MilvusConnection::ReleaseCollection(const proto::milvus::ReleaseCollectionRequest& request,
                                    proto::common::Status& response) {
    return Status::OK();
}

Status
MilvusConnection::DescribeCollection(const proto::milvus::DescribeCollectionRequest& request,
                                     proto::milvus::DescribeCollectionResponse& response) {
    return Status::OK();
}

Status
MilvusConnection::GetCollectionStats(const proto::milvus::GetCollectionStatisticsRequest& request,
                                     proto::milvus::GetCollectionStatisticsResponse& response) {
    return Status::OK();
}

Status
MilvusConnection::ShowCollections(const proto::milvus::ShowCollectionsRequest& request,
                                  proto::milvus::ShowCollectionsResponse& response) {
    return Status::OK();
}

}  // namespace milvus
