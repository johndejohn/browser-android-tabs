// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/sync/model/model_type_store_test_util.h"

#include <utility>

#include "base/bind.h"
#include "base/threading/thread_task_runner_handle.h"
#include "components/sync/base/model_type.h"
#include "components/sync/model_impl/blocking_model_type_store_impl.h"
#include "components/sync/model_impl/model_type_store_backend.h"
#include "components/sync/model_impl/model_type_store_impl.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace syncer {
namespace {

// Implementation of ModelTypeStore that delegates all calls to another
// instance, as injected in the constructor, useful for APIs that take ownership
// of ModelTypeStore.
class ForwardingModelTypeStore : public ModelTypeStore {
 public:
  explicit ForwardingModelTypeStore(ModelTypeStore* other) : other_(other) {}

  void ReadData(const IdList& id_list, ReadDataCallback callback) override {
    other_->ReadData(id_list, std::move(callback));
  }

  void ReadAllData(ReadAllDataCallback callback) override {
    other_->ReadAllData(std::move(callback));
  }

  void ReadAllMetadata(ReadMetadataCallback callback) override {
    other_->ReadAllMetadata(std::move(callback));
  }

  std::unique_ptr<WriteBatch> CreateWriteBatch() override {
    return other_->CreateWriteBatch();
  }

  void CommitWriteBatch(std::unique_ptr<WriteBatch> write_batch,
                        CallbackWithResult callback) override {
    other_->CommitWriteBatch(std::move(write_batch), std::move(callback));
  }

  void DeleteAllDataAndMetadata(CallbackWithResult callback) override {
    other_->DeleteAllDataAndMetadata(std::move(callback));
  }

 private:
  ModelTypeStore* other_;
};

}  // namespace

// static
std::unique_ptr<ModelTypeStore>
ModelTypeStoreTestUtil::CreateInMemoryStoreForTest(ModelType type) {
  return std::make_unique<ModelTypeStoreImpl>(
      type,
      std::make_unique<BlockingModelTypeStoreImpl>(
          type, ModelTypeStoreBackend::GetOrCreateInMemoryForTest()),
      base::ThreadTaskRunnerHandle::Get());
}

// static
RepeatingModelTypeStoreFactory
ModelTypeStoreTestUtil::FactoryForInMemoryStoreForTest() {
  return base::BindRepeating(
      [](ModelType type, ModelTypeStore::InitCallback callback) {
        std::move(callback).Run(/*error=*/base::nullopt,
                                CreateInMemoryStoreForTest(type));
      });
}

// static
void ModelTypeStoreTestUtil::MoveStoreToCallback(
    std::unique_ptr<ModelTypeStore> store,
    ModelType type,
    ModelTypeStore::InitCallback callback) {
  ASSERT_TRUE(store);
  std::move(callback).Run(/*error=*/base::nullopt, std::move(store));
}

// static
RepeatingModelTypeStoreFactory
ModelTypeStoreTestUtil::FactoryForForwardingStore(ModelTypeStore* target) {
  return base::BindRepeating(
      [](ModelTypeStore* target, ModelType,
         ModelTypeStore::InitCallback callback) {
        std::move(callback).Run(
            /*error=*/base::nullopt,
            std::make_unique<ForwardingModelTypeStore>(target));
      },
      base::Unretained(target));
}

}  // namespace syncer
