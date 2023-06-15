/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_CLOUD_SYNC_SA_DATA_HANDER_MOCK_H
#define OHOS_CLOUD_SYNC_SA_DATA_HANDER_MOCK_H

#include "data_handler.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
namespace OHOS::FileManagement::CloudSync::Test {
using namespace testing;
using namespace testing::ext;
using namespace std;
class DataHandlerMock final : public DataHandler {
public:
    void GetFetchCondition(FetchCondition &cond) {}
    int32_t OnFetchRecords(const std::shared_ptr<std::vector<DriveKit::DKRecord>> &records,
                                   std::vector<DriveKit::DKDownloadAsset> &outAssetsToDownload,
                                   std::shared_ptr<std::function<void(std::shared_ptr<DriveKit::DKContext>,
                                   std::shared_ptr<const DriveKit::DKDatabase>,
                                   const std::map<DriveKit::DKDownloadAsset, DriveKit::DKDownloadResult> &,
                                   const DriveKit::DKError &)>> &resultCallback) {return 0;}
    int32_t GetRetryRecords(std::vector<DriveKit::DKRecordId> &records) {return 0;}

    int32_t GetDownloadAsset(std::string cloudId,
                                       std::vector<DriveKit::DKDownloadAsset> &outAssetsToDownload) {return 0;}
    /* upload */
    int32_t GetCreatedRecords(std::vector<DriveKit::DKRecord> &records) {return 0;}
    int32_t GetDeletedRecords(std::vector<DriveKit::DKRecord> &records) {return 0;}
    int32_t GetMetaModifiedRecords(std::vector<DriveKit::DKRecord> &records) {return 0;}
    int32_t GetFileModifiedRecords(std::vector<DriveKit::DKRecord> &records) {return 0;}

    /* upload callback */
    int32_t OnCreateRecords(const std::map<DriveKit::DKRecordId,
        DriveKit::DKRecordOperResult> &map) {return 0;}
    int32_t OnDeleteRecords(const std::map<DriveKit::DKRecordId,
        DriveKit::DKRecordOperResult> &map) {return 0;}
    int32_t OnModifyMdirtyRecords(const std::map<DriveKit::DKRecordId,
        DriveKit::DKRecordOperResult> &map) {return 0;}
    int32_t OnModifyFdirtyRecords(const std::map<DriveKit::DKRecordId,
        DriveKit::DKRecordOperResult> &map) {return 0;}
    int32_t OnDownloadSuccess(const DriveKit::DKDownloadAsset &asset) {return 0;}
    int32_t Clean(const int action) {return 0;}

    /* cursor */
    int32_t SetCursor() {return 0;}
    int32_t GetCursor() {return 0;}
};
}
#endif