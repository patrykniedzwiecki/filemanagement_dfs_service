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

#ifndef DRIVE_KIT_DATABASE_H
#define DRIVE_KIT_DATABASE_H

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "dk_asset_read_session.h"
#include "dk_assets_downloader.h"
#include "dk_cloud_callback.h"
#include "dk_context.h"
#include "dk_error.h"
#include "dk_record.h"
#include "dk_result.h"

namespace DriveKit {
enum class DKSavePolicy {
    DK_SAVE_IF_UNCHANGED = 0,
    DK_CURRENT_WRITER_WINS,
};

struct DKPredicate {
    std::string predicateFormat;
};
struct DKSortDescriptor {
    bool ascending;
    DKFieldKey key;
};
using DKSortDescriptors = std::vector<DKSortDescriptor>;
struct DKQuery {
    DKRecordType recordType;
    DKPredicate predicate;
    DKSortDescriptors sortDescriptors;
};

struct UploadObject {
    std::string id;
    int64_t start;
    int64_t length;
    int64_t number;
};

struct UploadResource {
    std::string resourceId;
    std::vector<UploadObject> uploadObject;
};

class DKRecordOperResult : public DKResult {
public:
    void SetDKRecord(DKRecord record)
    {
        record_ = record;
    }
    DKRecord GetDKRecord()
    {
        return record_;
    }

private:
    DKRecord record_;
};

struct DKLock {
    int lockInterval;
    std::string lockSessionId;
};
using DKFieldKeyArray = std::vector<DKFieldKey>;
using DKQueryCursor = std::string;
class DKContainer;
class DKDatabase : public std::enable_shared_from_this<DKDatabase> {
public:
    DKDatabase() = default;
    ~DKDatabase() = default;

    using SaveRecordsCallback = std::function<void(std::shared_ptr<DKContext>,
                                                   std::shared_ptr<const DKDatabase>,
                                                   std::shared_ptr<const std::map<DKRecordId, DKRecordOperResult>>,
                                                   const DKError &)>;
    DKLocalErrorCode SaveRecords(std::shared_ptr<DKContext> context,
                                 std::vector<DKRecord> &records,
                                 DKSavePolicy policy,
                                 SaveRecordsCallback callback);

    using SaveRecordCallback = std::function<void(std::shared_ptr<DKContext>,
                                                  std::shared_ptr<const DKDatabase>,
                                                  DKRecordId,
                                                  DKRecordOperResult,
                                                  const DKError &)>;
    DKLocalErrorCode SaveRecord(std::shared_ptr<DKContext> context,
                                DKRecord &record,
                                DKSavePolicy policy,
                                SaveRecordCallback callback);

    using FetchRecordsCallback = std::function<void(std::shared_ptr<DKContext>,
                                                    std::shared_ptr<const DKDatabase>,
                                                    std::shared_ptr<const std::vector<DKRecord>>,
                                                    DKQueryCursor nextcursor,
                                                    const DKError &)>;
    DKLocalErrorCode FetchRecords(std::shared_ptr<DKContext> context,
                                  DKRecordType recordType,
                                  DKFieldKeyArray &desiredKeys,
                                  int resultLimit,
                                  DKQueryCursor cursor,
                                  FetchRecordsCallback callback);

    using FetchRecordCallback = std::function<void(std::shared_ptr<DKContext>,
                                                   std::shared_ptr<const DKDatabase>,
                                                   DKRecordId,
                                                   const DKRecord &,
                                                   const DKError &)>;
    DKLocalErrorCode FetchRecordWithId(std::shared_ptr<DKContext> context,
                                       DKRecordType recordType,
                                       DKRecordId recordId,
                                       DKFieldKeyArray &desiredKeys,
                                       FetchRecordCallback callback);

    using DeleteRecordsCallback = std::function<void(std::shared_ptr<DKContext>,
                                                     std::shared_ptr<const DKDatabase>,
                                                     std::shared_ptr<const std::map<DKRecordId, DKRecordOperResult>>,
                                                     const DKError &)>;
    DKLocalErrorCode DeleteRecords(std::shared_ptr<DKContext> context,
                                   std::vector<DKRecord> &records,
                                   DKSavePolicy policy,
                                   DeleteRecordsCallback callback);

    using ModifyRecordsCallback =
        std::function<void(std::shared_ptr<DKContext>,
                           std::shared_ptr<const DKDatabase>,
                           std::shared_ptr<const std::map<DKRecordId, DKRecordOperResult>> saveResult,
                           std::shared_ptr<const std::map<DKRecordId, DKRecordOperResult>> delResult,
                           const DKError &)>;
    DKLocalErrorCode ModifyRecords(std::shared_ptr<DKContext> context,
                                   std::vector<DKRecord> &recordsToSave,
                                   std::vector<DKRecord> &recordsToDelete,
                                   DKSavePolicy policy,
                                   bool atomically,
                                   ModifyRecordsCallback callback);

    DKLocalErrorCode FetchRecordsWithQuery(std::shared_ptr<DKContext> context,
                                           DKFieldKeyArray &desiredKeys,
                                           DKQuery query,
                                           int resultLimit,
                                           DKQueryCursor cursor,
                                           FetchRecordsCallback callback);

    using FetchDatabaseCallback = std::function<void(std::shared_ptr<DKContext>,
                                                     std::shared_ptr<const DKDatabase>,
                                                     std::shared_ptr<const std::map<DKRecordId, DKRecord>>,
                                                     DKQueryCursor nextcursor,
                                                     bool hasMore,
                                                     const DKError &)>;
    DKLocalErrorCode FetchDatabaseChanges(std::shared_ptr<DKContext> context,
                                          DKRecordType recordType,
                                          DKFieldKeyArray &desiredKeys,
                                          int resultLimit,
                                          DKQueryCursor cursor,
                                          FetchDatabaseCallback callback);

    DKError GetStartCursor(DKRecordType recordType, DKQueryCursor &cursor);
    DKError GenerateIds(int count, std::vector<DKRecordId> &ids);
    DKError GetLock(DKLock &lock);
    void DeleteLock(DKLock lock);

    std::shared_ptr<DKAssetsDownloader> GetAssetsDownloader();
    std::shared_ptr<DKAssetReadSession>
        NewAssetReadSession(DKRecordType recordType, DKRecordId recordId, DKFieldKey assetKey, DKAssetPath assetPath);

private:
    DKContainerName containerName_;
};
} // namespace DriveKit
#endif